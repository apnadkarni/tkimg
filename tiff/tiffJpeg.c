/* $Header$ */

/*
 * Copyright (c) 1994-1996 Sam Leffler
 * Copyright (c) 1994-1996 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#include "tiffInt.h"
#include "jpegtcl.h"
#include <assert.h>
#include <setjmp.h>

/*
 * TIFF Library
 *
 * JPEG Compression support per TIFF Technical Note #2
 * (*not* per the original TIFF 6.0 spec).
 *
 * This file is simply an interface to the libjpeg library written by
 * the Independent JPEG Group.  You need release 5 or later of the IJG
 * code, which you can find on the Internet at ftp.uu.net:/graphics/jpeg/.
 *
 * Contributed by Tom Lane <tgl@sss.pgh.pa.us>.
 */

/*
 * On some machines it may be worthwhile to use _setjmp or sigsetjmp
 * in place of plain setjmp.  These macros will make it easier.
 */
#define SETJMP(jbuf)		setjmp(jbuf)
#define LONGJMP(jbuf,code)	longjmp(jbuf,code)
#define JMP_BUF			jmp_buf

typedef struct jpeg_destination_mgr jpeg_destination_mgr;
typedef struct jpeg_source_mgr jpeg_source_mgr;
typedef	struct jpeg_error_mgr jpeg_error_mgr;

/*
 * State block for each open TIFF file using
 * libjpeg to do JPEG compression/decompression.
 *
 * libjpeg's visible state is either a jpeg_compress_struct
 * or jpeg_decompress_struct depending on which way we
 * are going.  comm can be used to refer to the fields
 * which are common to both.
 *
 * NB: cinfo is required to be the first member of JPEGState,
 *     so we can safely cast JPEGState* -> jpeg_xxx_struct*
 *     and vice versa!
 */
typedef	struct {
  union {
    struct jpeg_compress_struct c;
    struct jpeg_decompress_struct d;
    struct jpeg_common_struct comm;
  } cinfo;			/* NB: must be first */
  jpeg_error_mgr	err;		/* libjpeg error manager */
  JMP_BUF		exit_jmpbuf;	/* for catching libjpeg failures */
  /*
   * The following two members could be a union, but
   * they're small enough that it's not worth the effort.
   */
  jpeg_destination_mgr dest;	/* data dest for compression */
  jpeg_source_mgr	src;		/* data source for decompression */
					/* private state */
  TIFF*		tif;		/* back link needed by some code */
  uint16		photometric;	/* copy of PhotometricInterpretation */
  uint16		h_sampling;	/* luminance sampling factors */
  uint16		v_sampling;
  tsize_t		bytesperline;	/* decompressed bytes per scanline */
  /* pointers to intermediate buffers when processing downsampled data */
  JSAMPARRAY	ds_buffer[MAX_COMPONENTS];
  int		scancount;	/* number of "scanlines" accumulated */
  int		samplesperclump;

  TIFFVGetMethod	vgetparent;	/* super-class method */
  TIFFVSetMethod	vsetparent;	/* super-class method */
  TIFFStripMethod	defsparent;	/* super-class method */
  TIFFTileMethod	deftparent;	/* super-class method */
					/* pseudo-tag fields */
  void*		jpegtables;	/* JPEGTables tag value, or NULL */
  uint32	jpegtables_length; /* number of bytes in same */
  int		jpegquality;	/* Compression quality level */
  int		jpegcolormode;	/* Auto RGB<=>YCbCr convert? */
  int		jpegtablesmode;	/* What to put in JPEGTables */
} JPEGState;

#define	JState(tif)	((JPEGState*)(tif)->tif_data)

static int JPEGDecode(TIFF*, tidata_t, tsize_t, tsample_t);
static int JPEGDecodeRaw(TIFF*, tidata_t, tsize_t, tsample_t);
static int JPEGEncode(TIFF*, tidata_t, tsize_t, tsample_t);
static int JPEGEncodeRaw(TIFF*, tidata_t, tsize_t, tsample_t);

#define	FIELD_JPEGTABLES	(FIELD_CODEC+0)

static const TIFFFieldInfo jpegFieldInfo[] = {
    { TIFFTAG_JPEGTABLES,	 -1,-1,	TIFF_UNDEFINED,	FIELD_JPEGTABLES,
      FALSE,	TRUE,	"JPEGTables" },
    { TIFFTAG_JPEGQUALITY,	 0, 0,	TIFF_ANY,	FIELD_PSEUDO,
      TRUE,	FALSE,	"" },
    { TIFFTAG_JPEGCOLORMODE,	 0, 0,	TIFF_ANY,	FIELD_PSEUDO,
      FALSE,	FALSE,	"" },
    { TIFFTAG_JPEGTABLESMODE,	 0, 0,	TIFF_ANY,	FIELD_PSEUDO,
      FALSE,	FALSE,	"" },
};
#define	N(a)	(sizeof (a) / sizeof (a[0]))

/*
 * libjpeg interface layer.
 *
 * We use setjmp/longjmp to return control to libtiff
 * when a fatal error is encountered within the JPEG
 * library.  We also direct libjpeg error and warning
 * messages through the appropriate libtiff handlers.
 */

/*
 * Error handling routines (these replace corresponding
 * IJG routines from jerror.c).  These are used for both
 * compression and decompression.
 */

static void TIFFjpeg_error_exit(j_common_ptr);
static void TIFFjpeg_output_message(j_common_ptr);
static int TIFFjpeg_create_compress(JPEGState*);
static int TIFFjpeg_create_decompress(JPEGState*);
static int TIFFjpeg_set_defaults(JPEGState*);
static int TIFFjpeg_set_colorspace(JPEGState*, J_COLOR_SPACE);
static int TIFFjpeg_set_quality(JPEGState*, int, boolean);
static int TIFFjpeg_suppress_tables(JPEGState*, boolean);
static int TIFFjpeg_start_compress(JPEGState*, boolean);
static int TIFFjpeg_write_scanlines(JPEGState*, JSAMPARRAY, int);
static int TIFFjpeg_write_raw_data(JPEGState*, JSAMPIMAGE, int);
static int TIFFjpeg_finish_compress(JPEGState* sp);
static int TIFFjpeg_write_tables(JPEGState*);
static int TIFFjpeg_read_header(JPEGState*, boolean);
static int TIFFjpeg_start_decompress(JPEGState*);
static int TIFFjpeg_read_scanlines(JPEGState*, JSAMPARRAY, int);
static int  TIFFjpeg_read_raw_data(JPEGState*, JSAMPIMAGE, int);
static int TIFFjpeg_finish_decompress(JPEGState*);
static int TIFFjpeg_abort(JPEGState*);
static int TIFFjpeg_destroy(JPEGState*);
static JSAMPARRAY TIFFjpeg_alloc_sarray(JPEGState*,
	int, JDIMENSION, JDIMENSION);
static void TIFFjpeg_data_dest(JPEGState*, TIFF*);
static int TIFFjpeg_tables_dest(JPEGState*, TIFF*);
static void TIFFjpeg_data_src(JPEGState*, TIFF*);
static void TIFFjpeg_tables_src(JPEGState*, TIFF*);

static void std_init_destination(j_compress_ptr);
static boolean std_empty_output_buffer(j_compress_ptr);
static void std_term_destination(j_compress_ptr);
static void tables_init_destination(j_compress_ptr);
static boolean tables_empty_output_buffer(j_compress_ptr);
static void tables_term_destination(j_compress_ptr);
static void std_init_source(j_decompress_ptr);
static boolean std_fill_input_buffer(j_decompress_ptr);
static void std_skip_input_data(j_decompress_ptr, long);
static void std_term_source(j_decompress_ptr);
static void tables_init_source(j_decompress_ptr);
static int alloc_downsampled_buffers(TIFF*,
	jpeg_component_info*, int);

static int JPEGSetupDecode(TIFF* tif);
static int JPEGSetupEncode(TIFF* tif);
static int JPEGEncode(TIFF*, tidata_t, tsize_t, tsample_t);
static int JPEGEncodeRaw(TIFF*, tidata_t, tsize_t, tsample_t);
static int JPEGPostEncode(TIFF* tif);
static void JPEGCleanup(TIFF*);
static int JPEGVSetField(TIFF* tif, ttag_t tag, va_list ap);
static int JPEGVGetField(TIFF* tif, ttag_t tag, va_list ap);
static uint32 JPEGDefaultStripSize(TIFF*, uint32);
static void JPEGDefaultTileSize(TIFF*, uint32*, uint32*);


static void
TIFFjpeg_error_exit(cinfo)
    j_common_ptr cinfo;
{
    JPEGState *sp = (JPEGState *) cinfo;	/* NB: cinfo assumed first */
    char buffer[JMSG_LENGTH_MAX];

    (*cinfo->err->format_message) (cinfo, buffer);
    TIFFError("JPEGLib", buffer);		/* display the error message */
    jpeg_abort(cinfo);			/* clean up libjpeg state */
    LONGJMP(sp->exit_jmpbuf, 1);		/* return to libtiff caller */
}

/*
 * This routine is invoked only for warning messages,
 * since error_exit does its own thing and trace_level
 * is never set > 0.
 */

static void
TIFFjpeg_output_message(cinfo)
    j_common_ptr cinfo;
{
}

/*
 * Interface routines.  This layer of routines exists
 * primarily to limit side-effects from using setjmp.
 * Also, normal/error returns are converted into return
 * values per libtiff practice.
 */
#define	CALLJPEG(sp, fail, op)	(SETJMP((sp)->exit_jmpbuf) ? (fail) : (op))

#define	CALLVJPEG(sp, op)	CALLJPEG(sp, 0, ((op),1))
#undef  CALLVJPEG
#define CALLVJPEG(sp, op)       (SETJMP((sp)->exit_jmpbuf) ? (0) : ((op),1))

static int
TIFFjpeg_create_compress(sp)
    JPEGState* sp;
{
    /* initialize JPEG error handling */
    sp->cinfo.c.err = jpeg_std_error(&sp->err);
    sp->err.error_exit = TIFFjpeg_error_exit;
    sp->err.output_message = TIFFjpeg_output_message;

    return CALLVJPEG(sp, jpeg_CreateCompress(&sp->cinfo.c, JPEG_LIB_VERSION,
			(size_t) sizeof(struct jpeg_compress_struct)));
}


static int
TIFFjpeg_create_decompress(sp)
    JPEGState* sp;
{
	/* initialize JPEG error handling */
	sp->cinfo.d.err = jpeg_std_error(&sp->err);
	sp->err.error_exit = TIFFjpeg_error_exit;
	sp->err.output_message = TIFFjpeg_output_message;

	return CALLVJPEG(sp, jpeg_CreateDecompress(&sp->cinfo.d, JPEG_LIB_VERSION,
		(size_t) sizeof(struct jpeg_decompress_struct)));
}


static int
TIFFjpeg_set_defaults(sp)
    JPEGState* sp;
{
    return CALLVJPEG(sp, jpeg_set_defaults(&sp->cinfo.c));
}


static int
TIFFjpeg_set_colorspace(sp, colorspace)
    JPEGState* sp;
    J_COLOR_SPACE colorspace;
{
    return CALLVJPEG(sp, jpeg_set_colorspace(&sp->cinfo.c, colorspace));
}


static int
TIFFjpeg_set_quality(sp, quality, force_baseline)
    JPEGState* sp;
    int quality;
    boolean force_baseline;
{
    return CALLVJPEG(sp,
     jpeg_set_quality(&sp->cinfo.c, quality, force_baseline));
}


static int
TIFFjpeg_suppress_tables(sp, suppress)
    JPEGState* sp;
    boolean suppress;
{
    return CALLVJPEG(sp, jpeg_suppress_tables(&sp->cinfo.c, suppress));
}

static int
TIFFjpeg_start_compress(sp, write_all_tables)
    JPEGState* sp;
    boolean write_all_tables;
{
    return CALLVJPEG(sp,
     jpeg_start_compress(&sp->cinfo.c, write_all_tables));
}

static int
TIFFjpeg_write_scanlines(sp, scanlines, num_lines)
    JPEGState* sp;
    JSAMPARRAY scanlines;
    int num_lines;
{
    return CALLJPEG(sp, -1, (int) jpeg_write_scanlines(&sp->cinfo.c,
	  scanlines, (JDIMENSION) num_lines));
}


static int
TIFFjpeg_write_raw_data(sp, data, num_lines)
    JPEGState* sp;
    JSAMPIMAGE data;
    int num_lines;
{
	return CALLJPEG(sp, -1, (int) jpeg_write_raw_data(&sp->cinfo.c,
	    data, (JDIMENSION) num_lines));
}


static int
TIFFjpeg_finish_compress(sp)
    JPEGState* sp;
{
    return CALLVJPEG(sp, jpeg_finish_compress(&sp->cinfo.c));
}


static int
TIFFjpeg_write_tables(sp)
    JPEGState* sp;
{
    return CALLVJPEG(sp, jpeg_write_tables(&sp->cinfo.c));
}


static int
TIFFjpeg_read_header(sp, require_image)
    JPEGState* sp;
    boolean require_image;
{
    return CALLJPEG(sp, -1, jpeg_read_header(&sp->cinfo.d, require_image));
}

static int
TIFFjpeg_start_decompress(sp)
    JPEGState* sp;
{
    return CALLVJPEG(sp, jpeg_start_decompress(&sp->cinfo.d));
}


static int
TIFFjpeg_read_scanlines(sp, scanlines, max_lines)
    JPEGState* sp;
    JSAMPARRAY scanlines;
    int max_lines;
{
    return CALLJPEG(sp, -1, (int) jpeg_read_scanlines(&sp->cinfo.d,
	    scanlines, (JDIMENSION) max_lines));
}

static int
TIFFjpeg_read_raw_data(sp, data, max_lines)
    JPEGState* sp;
    JSAMPIMAGE data;
    int max_lines;
{
    return CALLJPEG(sp, -1, (int) jpeg_read_raw_data(&sp->cinfo.d,
	    data, (JDIMENSION) max_lines));
}

static int
TIFFjpeg_finish_decompress(sp)
    JPEGState* sp;
{
    return CALLJPEG(sp, -1, (int) jpeg_finish_decompress(&sp->cinfo.d));
}

static int
TIFFjpeg_abort(sp)
    JPEGState* sp;
{
    return CALLVJPEG(sp, jpeg_abort(&sp->cinfo.comm));
}

static int
TIFFjpeg_destroy(sp)
    JPEGState* sp;
{
    return CALLVJPEG(sp, jpeg_destroy(&sp->cinfo.comm));
}

static JSAMPARRAY
TIFFjpeg_alloc_sarray(sp, pool_id, samplesperrow, numrows)
    JPEGState* sp;
    int pool_id;
    JDIMENSION samplesperrow;
    JDIMENSION numrows;
{
	return CALLJPEG(sp, (JSAMPARRAY) NULL,
	    (*sp->cinfo.comm.mem->alloc_sarray)
		(&sp->cinfo.comm, pool_id, samplesperrow, numrows));
}

/*
 * JPEG library destination data manager.
 * These routines direct compressed data from libjpeg into the
 * libtiff output buffer.
 */

static void
std_init_destination(cinfo)
    j_compress_ptr cinfo;
{
    JPEGState* sp = (JPEGState*) cinfo;
    TIFF* tif = sp->tif;

    sp->dest.next_output_byte = (JOCTET*) tif->tif_rawdata;
    sp->dest.free_in_buffer = (size_t) tif->tif_rawdatasize;
}

static boolean
std_empty_output_buffer(cinfo)
    j_compress_ptr cinfo;
{
    JPEGState* sp = (JPEGState*) cinfo;
    TIFF* tif = sp->tif;

    /* the entire buffer has been filled */
    tif->tif_rawcc = tif->tif_rawdatasize;
    TIFFFlushData1(tif);
    sp->dest.next_output_byte = (JOCTET*) tif->tif_rawdata;
    sp->dest.free_in_buffer = (size_t) tif->tif_rawdatasize;

    return (TRUE);
}

static void
std_term_destination(cinfo)
    j_compress_ptr cinfo;
{
    JPEGState* sp = (JPEGState*) cinfo;
    TIFF* tif = sp->tif;

    tif->tif_rawcp = (tidata_t) sp->dest.next_output_byte;
    tif->tif_rawcc =
      tif->tif_rawdatasize - (tsize_t) sp->dest.free_in_buffer;
    /* NB: libtiff does the final buffer flush */
}

static void
TIFFjpeg_data_dest(sp, tif)
    JPEGState* sp;
    TIFF* tif;
{
    (void) tif;
    sp->cinfo.c.dest = &sp->dest;
    sp->dest.init_destination = std_init_destination;
    sp->dest.empty_output_buffer = std_empty_output_buffer;
    sp->dest.term_destination = std_term_destination;
}

/*
 * Alternate destination manager for outputting to JPEGTables field.
 */

static void
tables_init_destination(cinfo)
    j_compress_ptr cinfo;
{
    JPEGState* sp = (JPEGState*) cinfo;

    /* while building, jpegtables_length is allocated buffer size */
    sp->dest.next_output_byte = (JOCTET*) sp->jpegtables;
    sp->dest.free_in_buffer = (size_t) sp->jpegtables_length;
}

static boolean
tables_empty_output_buffer(cinfo)
    j_compress_ptr cinfo;
{
    JPEGState* sp = (JPEGState*) cinfo;
    void* newbuf;

    /* the entire buffer has been filled; enlarge it by 1000 bytes */
    newbuf = TkimgTIFFrealloc((tdata_t) sp->jpegtables,
			      (tsize_t) (sp->jpegtables_length + 1000));
    if (newbuf == NULL)
      ERREXIT1(cinfo, JERR_OUT_OF_MEMORY, 100);
    sp->dest.next_output_byte = (JOCTET*) newbuf + sp->jpegtables_length;
    sp->dest.free_in_buffer = (size_t) 1000;
    sp->jpegtables = newbuf;
    sp->jpegtables_length += 1000;
    return (TRUE);
}

static void
tables_term_destination(cinfo)
    j_compress_ptr cinfo;
{
    JPEGState* sp = (JPEGState*) cinfo;

    /* set tables length to number of bytes actually emitted */
    sp->jpegtables_length -= sp->dest.free_in_buffer;
}

static int
TIFFjpeg_tables_dest(sp, tif)
    JPEGState* sp;
    TIFF* tif;
{
    (void) tif;
    /*
     * Allocate a working buffer for building tables.
     * Initial size is 1000 bytes, which is usually adequate.
     */
    if (sp->jpegtables) {
        TkimgTIFFfree(sp->jpegtables);
    }
    sp->jpegtables_length = 1000;
    sp->jpegtables = (void*) TkimgTIFFmalloc((tsize_t) sp->jpegtables_length);
    if (sp->jpegtables == NULL) {
        sp->jpegtables_length = 0;
	TIFFError("TIFFjpeg_tables_dest", "No space for JPEGTables");
      return (0);
    }
    sp->cinfo.c.dest = &sp->dest;
    sp->dest.init_destination = tables_init_destination;
    sp->dest.empty_output_buffer = tables_empty_output_buffer;
    sp->dest.term_destination = tables_term_destination;
    return (1);
}

/*
 * JPEG library source data manager.
 * These routines supply compressed data to libjpeg.
 */

static void
std_init_source(cinfo)
    j_decompress_ptr cinfo;
{
    JPEGState* sp = (JPEGState*) cinfo;
    TIFF* tif = sp->tif;

    sp->src.next_input_byte = (const JOCTET*) tif->tif_rawdata;
    sp->src.bytes_in_buffer = (size_t) tif->tif_rawcc;
}

static boolean
std_fill_input_buffer(cinfo)
    j_decompress_ptr cinfo;
{
    JPEGState* sp = (JPEGState* ) cinfo;
    static const JOCTET dummy_EOI[2] = { 0xFF, JPEG_EOI };

    /*
     * Should never get here since entire strip/tile is
     * read into memory before the decompressor is called,
     * and thus was supplied by init_source.
     */
    WARNMS(cinfo, JWRN_JPEG_EOF);
    /* insert a fake EOI marker */
    sp->src.next_input_byte = dummy_EOI;
    sp->src.bytes_in_buffer = 2;
    return (TRUE);
}

static void
std_skip_input_data(cinfo, num_bytes)
    j_decompress_ptr cinfo;
    long num_bytes;
{
    JPEGState* sp = (JPEGState*) cinfo;

    if (num_bytes > 0) {
      if (num_bytes > (long) sp->src.bytes_in_buffer) {
	/* oops, buffer overrun */
	(void) std_fill_input_buffer(cinfo);
      } else {
	sp->src.next_input_byte += (size_t) num_bytes;
	sp->src.bytes_in_buffer -= (size_t) num_bytes;
      }
    }
}

static void
std_term_source(cinfo)
    j_decompress_ptr cinfo;
{
    /* No work necessary here */
    /* Or must we update tif->tif_rawcp, tif->tif_rawcc ??? */
    /* (if so, need empty tables_term_source!) */
    (void) cinfo;
}

static void
TIFFjpeg_data_src(sp, tif)
    JPEGState* sp;
    TIFF* tif;
{
    (void) tif;
    sp->cinfo.d.src = &sp->src;
    sp->src.init_source = std_init_source;
    sp->src.fill_input_buffer = std_fill_input_buffer;
    sp->src.skip_input_data = std_skip_input_data;
    sp->src.resync_to_restart = jpeg_resync_to_restart;
    sp->src.term_source = std_term_source;
    sp->src.bytes_in_buffer = 0;		/* for safety */
    sp->src.next_input_byte = NULL;
}

/*
 * Alternate source manager for reading from JPEGTables.
 * We can share all the code except for the init routine.
 */

static void
tables_init_source(cinfo)
    j_decompress_ptr cinfo;
{
    JPEGState* sp = (JPEGState*) cinfo;

    sp->src.next_input_byte = (const JOCTET*) sp->jpegtables;
    sp->src.bytes_in_buffer = (size_t) sp->jpegtables_length;
}

static void
TIFFjpeg_tables_src(sp, tif)
    JPEGState* sp;
    TIFF* tif;
{
    TIFFjpeg_data_src(sp, tif);
    sp->src.init_source = tables_init_source;
}

/*
 * Allocate downsampled-data buffers needed for downsampled I/O.
 * We use values computed in jpeg_start_compress or jpeg_start_decompress.
 * We use libjpeg's allocator so that buffers will be released automatically
 * when done with strip/tile.
 * This is also a handy place to compute samplesperclump, bytesperline.
 */

static int
alloc_downsampled_buffers(tif, comp_info, num_components)
    TIFF* tif;
    jpeg_component_info* comp_info;
    int num_components;
{
	JPEGState* sp = JState(tif);
	int ci;
	jpeg_component_info* compptr;
	JSAMPARRAY buf;
	int samples_per_clump = 0;

	for (ci = 0, compptr = comp_info; ci < num_components;
	     ci++, compptr++) {
		samples_per_clump += compptr->h_samp_factor *
			compptr->v_samp_factor;
		buf = TIFFjpeg_alloc_sarray(sp, JPOOL_IMAGE,
				compptr->width_in_blocks * DCTSIZE,
				(JDIMENSION) (compptr->v_samp_factor*DCTSIZE));
		if (buf == NULL)
			return (0);
		sp->ds_buffer[ci] = buf;
	}
	sp->samplesperclump = samples_per_clump;
	/* Cb,Cr both have sampling factors 1 */
	/* so downsampled width of Cb is # of clumps per line */
	sp->bytesperline = sizeof(JSAMPLE) * samples_per_clump *
		comp_info[1].downsampled_width;
	return (1);
}


/*
 * JPEG Decoding.
 */

static int
JPEGSetupDecode(tif)
    TIFF* tif;
{
	JPEGState* sp = JState(tif);
	TIFFDirectory *td = &tif->tif_dir;

	assert(sp != NULL);
	assert(sp->cinfo.comm.is_decompressor);

	/* Read JPEGTables if it is present */
	if (TIFFFieldSet(tif,FIELD_JPEGTABLES)) {
		TIFFjpeg_tables_src(sp, tif);
		if(TIFFjpeg_read_header(sp,FALSE) != JPEG_HEADER_TABLES_ONLY) {
			TIFFError("JPEGSetupDecode", "Bogus JPEGTables field");
			return (0);
		}
	}

	/* Grab parameters that are same for all strips/tiles */
	sp->photometric = td->td_photometric;
	switch (sp->photometric) {
	case PHOTOMETRIC_YCBCR:
		sp->h_sampling = td->td_ycbcrsubsampling[0];
		sp->v_sampling = td->td_ycbcrsubsampling[1];
		break;
	default:
		/* TIFF 6.0 forbids subsampling of all other color spaces */
		sp->h_sampling = 1;
		sp->v_sampling = 1;
		break;
	}

	/* Set up for reading normal data */
	TIFFjpeg_data_src(sp, tif);
	tif->tif_postdecode = _TIFFNoPostDecode; /* override byte swapping */
	return (1);
}

/*
 * Set up for decoding a strip or tile.
 */

static int
JPEGPreDecode (
    TIFF* tif,
    tsample_t s)
{
	JPEGState *sp = JState(tif);
	TIFFDirectory *td = &tif->tif_dir;
	static char module[] = "JPEGPreDecode";
	uint32 segment_width, segment_height;
	int downsampled_output;
	int ci;

	assert(sp != NULL);
	assert(sp->cinfo.comm.is_decompressor);
	/*
	 * Reset decoder state from any previous strip/tile,
	 * in case application didn't read the whole strip.
	 */
	if (!TIFFjpeg_abort(sp))
		return (0);
	/*
	 * Read the header for this strip/tile.
	 */
	if (TIFFjpeg_read_header(sp, TRUE) != JPEG_HEADER_OK)
		return (0);
	/*
	 * Check image parameters and set decompression parameters.
	 */
	if (isTiled(tif)) {
		segment_width = td->td_tilewidth;
		segment_height = td->td_tilelength;
		sp->bytesperline = TIFFTileRowSize(tif);
	} else {
		segment_width = td->td_imagewidth;
		segment_height = td->td_imagelength - tif->tif_row;
		if (segment_height > td->td_rowsperstrip)
			segment_height = td->td_rowsperstrip;
		sp->bytesperline = TIFFScanlineSize(tif);
	}
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE && s > 0) {
		/*
		 * For PC 2, scale down the expected strip/tile size
		 * to match a downsampled component
		 */
		segment_width = TIFFhowmany(segment_width, sp->h_sampling);
		segment_height = TIFFhowmany(segment_height, sp->v_sampling);
	}
	if (sp->cinfo.d.image_width != segment_width ||
	    sp->cinfo.d.image_height != segment_height) {
		TIFFError(module, "Improper JPEG strip/tile size");
		return (0);
	}
	if (sp->cinfo.d.num_components !=
	    (td->td_planarconfig == PLANARCONFIG_CONTIG ?
	     td->td_samplesperpixel : 1)) {
		TIFFError(module, "Improper JPEG component count");
		return (0);
	}
	if (sp->cinfo.d.data_precision != td->td_bitspersample) {
		TIFFError(module, "Improper JPEG data precision");
		return (0);
	}
	if (td->td_planarconfig == PLANARCONFIG_CONTIG) {
		/* Component 0 should have expected sampling factors */
		if (sp->cinfo.d.comp_info[0].h_samp_factor != sp->h_sampling ||
		    sp->cinfo.d.comp_info[0].v_samp_factor != sp->v_sampling) {
			TIFFError(module, "Improper JPEG sampling factors");
			return (0);
		}
		/* Rest should have sampling factors 1,1 */
		for (ci = 1; ci < sp->cinfo.d.num_components; ci++) {
			if (sp->cinfo.d.comp_info[ci].h_samp_factor != 1 ||
			    sp->cinfo.d.comp_info[ci].v_samp_factor != 1) {
				TIFFError(module, "Improper JPEG sampling factors");
				return (0);
			}
		}
	} else {
		/* PC 2's single component should have sampling factors 1,1 */
		if (sp->cinfo.d.comp_info[0].h_samp_factor != 1 ||
		    sp->cinfo.d.comp_info[0].v_samp_factor != 1) {
			TIFFError(module, "Improper JPEG sampling factors");
			return (0);
		}
	}
	downsampled_output = FALSE;
	if (td->td_planarconfig == PLANARCONFIG_CONTIG &&
	    sp->photometric == PHOTOMETRIC_YCBCR &&
	    sp->jpegcolormode == JPEGCOLORMODE_RGB) {
		/* Convert YCbCr to RGB */
		sp->cinfo.d.jpeg_color_space = JCS_YCbCr;
		sp->cinfo.d.out_color_space = JCS_RGB;
	} else {
		/* Suppress colorspace handling */
		sp->cinfo.d.jpeg_color_space = JCS_UNKNOWN;
		sp->cinfo.d.out_color_space = JCS_UNKNOWN;
		if (td->td_planarconfig == PLANARCONFIG_CONTIG &&
		    (sp->h_sampling != 1 || sp->v_sampling != 1))
			downsampled_output = TRUE;
		/* XXX what about up-sampling? */
	}
	if (downsampled_output) {
		/* Need to use raw-data interface to libjpeg */
		sp->cinfo.d.raw_data_out = TRUE;
		tif->tif_decoderow = JPEGDecodeRaw;
		tif->tif_decodestrip = JPEGDecodeRaw;
		tif->tif_decodetile = JPEGDecodeRaw;
	} else {
		/* Use normal interface to libjpeg */
		sp->cinfo.d.raw_data_out = FALSE;
		tif->tif_decoderow = JPEGDecode;
		tif->tif_decodestrip = JPEGDecode;
		tif->tif_decodetile = JPEGDecode;
	}
	/* Start JPEG decompressor */
	if (!TIFFjpeg_start_decompress(sp))
		return (0);
	/* Allocate downsampled-data buffers if needed */
	if (downsampled_output) {
		if (!alloc_downsampled_buffers(tif, sp->cinfo.d.comp_info,
					       sp->cinfo.d.num_components))
			return (0);
		sp->scancount = DCTSIZE;	/* mark buffer empty */
	}
	return (1);
}

/*
 * Decode a chunk of pixels.
 * "Standard" case: returned data is not downsampled.
 */

static int
JPEGDecode (
    TIFF* tif,
    tidata_t buf,
    tsize_t cc,
    tsample_t s)
{
	JPEGState *sp = JState(tif);
	tsize_t nrows;
	JSAMPROW bufptr[1];

	(void) s;
	assert(sp != NULL);
	/* data is expected to be read in multiples of a scanline */
	nrows = cc / sp->bytesperline;

	while (nrows-- > 0) {
		bufptr[0] = (JSAMPROW) buf;
		if (TIFFjpeg_read_scanlines(sp, bufptr, 1) != 1)
			return (0);
		if (nrows > 0)
			tif->tif_row++;
		buf += sp->bytesperline;
	}
	/* Close down the decompressor if we've finished the strip or tile. */
	if (sp->cinfo.d.output_scanline == sp->cinfo.d.output_height) {
		if (TIFFjpeg_finish_decompress(sp) != TRUE)
			return (0);
	}
	return (1);
}

/*
 * Decode a chunk of pixels.
 * Returned data is downsampled per sampling factors.
 */

static int
JPEGDecodeRaw (
    TIFF* tif,
    tidata_t buf,
    tsize_t cc,
    tsample_t s)
{
	JPEGState *sp = JState(tif);
	JSAMPLE* inptr;
	JSAMPLE* outptr;
	tsize_t nrows;
	JDIMENSION clumps_per_line, nclump;
	int clumpoffset, ci, xpos, ypos;
	jpeg_component_info* compptr;
	int samples_per_clump = sp->samplesperclump;

	(void) s;
	assert(sp != NULL);
	/* data is expected to be read in multiples of a scanline */
	nrows = cc / sp->bytesperline;

	/* Cb,Cr both have sampling factors 1, so this is correct */
	clumps_per_line = sp->cinfo.d.comp_info[1].downsampled_width;

	while (nrows-- > 0) {
		/* Reload downsampled-data buffer if needed */
		if (sp->scancount >= DCTSIZE) {
			int n = sp->cinfo.d.max_v_samp_factor * DCTSIZE;
			if (TIFFjpeg_read_raw_data(sp, sp->ds_buffer, n) != n)
				return (0);
			sp->scancount = 0;
			/* Close down the decompressor if done. */
			if (sp->cinfo.d.output_scanline >=
			    sp->cinfo.d.output_height) {
				if (TIFFjpeg_finish_decompress(sp) != TRUE)
					return (0);
			}
		}
		/*
		 * Fastest way to unseparate the data is to make one pass
		 * over the scanline for each row of each component.
		 */
		clumpoffset = 0;		/* first sample in clump */
		for (ci = 0, compptr = sp->cinfo.d.comp_info;
		     ci < sp->cinfo.d.num_components;
		     ci++, compptr++) {
		    int hsamp = compptr->h_samp_factor;
		    int vsamp = compptr->v_samp_factor;
		    for (ypos = 0; ypos < vsamp; ypos++) {
			inptr = sp->ds_buffer[ci][sp->scancount*vsamp + ypos];
			outptr = ((JSAMPLE*) buf) + clumpoffset;
			if (hsamp == 1) {
			    /* fast path for at least Cb and Cr */
			    for (nclump = clumps_per_line; nclump-- > 0; ) {
				outptr[0] = *inptr++;
				outptr += samples_per_clump;
			    }
			} else {
			    /* general case */
			    for (nclump = clumps_per_line; nclump-- > 0; ) {
				for (xpos = 0; xpos < hsamp; xpos++)
				    outptr[xpos] = *inptr++;
				outptr += samples_per_clump;
			    }
			}
			clumpoffset += hsamp;
		    }
		}
		sp->scancount++;
		if (nrows > 0)
			tif->tif_row++;
		buf += sp->bytesperline;
	}
	return (1);
}


/*
 * JPEG Encoding.
 */

static void unsuppress_quant_table(JPEGState*, int);
static void
unsuppress_quant_table(sp, tblno)
    JPEGState* sp;
    int tblno;
{
	JQUANT_TBL* qtbl;

	if ((qtbl = sp->cinfo.c.quant_tbl_ptrs[tblno]) != NULL)
		qtbl->sent_table = FALSE;
}

static void unsuppress_huff_table(JPEGState*, int);
static void
unsuppress_huff_table(sp, tblno)
    JPEGState* sp;
    int tblno;
{
	JHUFF_TBL* htbl;

	if ((htbl = sp->cinfo.c.dc_huff_tbl_ptrs[tblno]) != NULL)
		htbl->sent_table = FALSE;
	if ((htbl = sp->cinfo.c.ac_huff_tbl_ptrs[tblno]) != NULL)
		htbl->sent_table = FALSE;
}

static int prepare_JPEGTables(TIFF*);
static int
prepare_JPEGTables(tif)
    TIFF* tif;
{
	JPEGState* sp = JState(tif);

	/* Initialize quant tables for current quality setting */
	if (!TIFFjpeg_set_quality(sp, sp->jpegquality, FALSE))
		return (0);
	/* Mark only the tables we want for output */
	/* NB: chrominance tables are currently used only with YCbCr */
	if (!TIFFjpeg_suppress_tables(sp, TRUE))
		return (0);
	if (sp->jpegtablesmode & JPEGTABLESMODE_QUANT) {
		unsuppress_quant_table(sp, 0);
		if (sp->photometric == PHOTOMETRIC_YCBCR)
			unsuppress_quant_table(sp, 1);
	}
	if (sp->jpegtablesmode & JPEGTABLESMODE_HUFF) {
		unsuppress_huff_table(sp, 0);
		if (sp->photometric == PHOTOMETRIC_YCBCR)
			unsuppress_huff_table(sp, 1);
	}
	/* Direct libjpeg output into jpegtables */
	if (!TIFFjpeg_tables_dest(sp, tif))
		return (0);
	/* Emit tables-only datastream */
	if (!TIFFjpeg_write_tables(sp))
		return (0);

	return (1);
}

static int
JPEGSetupEncode(tif)
    TIFF* tif;
{
	JPEGState* sp = JState(tif);
	TIFFDirectory *td = &tif->tif_dir;
	static char module[] = "JPEGSetupEncode";

	assert(sp != NULL);
	assert(!sp->cinfo.comm.is_decompressor);

	/*
	 * Initialize all JPEG parameters to default values.
	 * Note that jpeg_set_defaults needs legal values for
	 * in_color_space and input_components.
	 */
	sp->cinfo.c.in_color_space = JCS_UNKNOWN;
	sp->cinfo.c.input_components = 1;
	if (!TIFFjpeg_set_defaults(sp))
		return (0);
	/* Set per-file parameters */
	sp->photometric = td->td_photometric;
	switch (sp->photometric) {
	case PHOTOMETRIC_YCBCR:
		sp->h_sampling = td->td_ycbcrsubsampling[0];
		sp->v_sampling = td->td_ycbcrsubsampling[1];
		/*
		 * A ReferenceBlackWhite field *must* be present since the
		 * default value is inappropriate for YCbCr.  Fill in the
		 * proper value if application didn't set it.
		 */
		{
			float *ref;
			if (!TIFFGetField(tif, TIFFTAG_REFERENCEBLACKWHITE,
					  &ref)) {
				float refbw[6];
				long top = 1L << td->td_bitspersample;
				refbw[0] = 0;
				refbw[1] = (float)(top-1L);
				refbw[2] = (float)(top>>1);
				refbw[3] = refbw[1];
				refbw[4] = refbw[2];
				refbw[5] = refbw[1];
				TIFFSetField(tif, TIFFTAG_REFERENCEBLACKWHITE,
					     refbw);
			}
		}
		break;
	case PHOTOMETRIC_PALETTE:		/* disallowed by Tech Note */
	case PHOTOMETRIC_MASK:
		TIFFError(module,
			  "PhotometricInterpretation %d not allowed for JPEG",
			  (int) sp->photometric);
		return (0);
	default:
		/* TIFF 6.0 forbids subsampling of all other color spaces */
		sp->h_sampling = 1;
		sp->v_sampling = 1;
		break;
	}

	/* Verify miscellaneous parameters */

	/*
	 * This would need work if libtiff ever supports different
	 * depths for different components, or if libjpeg ever supports
	 * run-time selection of depth.  Neither is imminent.
	 */
	if (td->td_bitspersample != BITS_IN_JSAMPLE) {
		TIFFError(module, "BitsPerSample %d not allowed for JPEG",
			  (int) td->td_bitspersample);
		return (0);
	}
	sp->cinfo.c.data_precision = td->td_bitspersample;
	if (isTiled(tif)) {
		if ((td->td_tilelength % (sp->v_sampling * DCTSIZE)) != 0) {
			TIFFError(module,
				  "JPEG tile height must be multiple of %d",
				  sp->v_sampling * DCTSIZE);
			return (0);
		}
		if ((td->td_tilewidth % (sp->h_sampling * DCTSIZE)) != 0) {
			TIFFError(module,
				  "JPEG tile width must be multiple of %d",
				  sp->h_sampling * DCTSIZE);
			return (0);
		}
	} else {
		if (td->td_rowsperstrip < td->td_imagelength &&
		    (td->td_rowsperstrip % (sp->v_sampling * DCTSIZE)) != 0) {
			TIFFError(module,
				  "RowsPerStrip must be multiple of %d for JPEG",
				  sp->v_sampling * DCTSIZE);
			return (0);
		}
	}

	/* Create a JPEGTables field if appropriate */
	if (sp->jpegtablesmode & (JPEGTABLESMODE_QUANT|JPEGTABLESMODE_HUFF)) {
		if (!prepare_JPEGTables(tif))
			return (0);
		/* Mark the field present */
		/* Can't use TIFFSetField since BEENWRITING is already set! */
		TIFFSetFieldBit(tif, FIELD_JPEGTABLES);
		tif->tif_flags |= TIFF_DIRTYDIRECT;
	} else {
		/* We do not support application-supplied JPEGTables, */
		/* so mark the field not present */
		TIFFClrFieldBit(tif, FIELD_JPEGTABLES);
	}

	/* Direct libjpeg output to libtiff's output buffer */
	TIFFjpeg_data_dest(sp, tif);

	return (1);
}

/*
 * Set encoding state at the start of a strip or tile.
 */

static int
JPEGPreEncode (
    TIFF* tif,
    tsample_t s)
{
	JPEGState *sp = JState(tif);
	TIFFDirectory *td = &tif->tif_dir;
	static char module[] = "JPEGPreEncode";
	uint32 segment_width, segment_height;
	int downsampled_input;

	assert(sp != NULL);
	assert(!sp->cinfo.comm.is_decompressor);
	/*
	 * Set encoding parameters for this strip/tile.
	 */
	if (isTiled(tif)) {
		segment_width = td->td_tilewidth;
		segment_height = td->td_tilelength;
		sp->bytesperline = TIFFTileRowSize(tif);
	} else {
		segment_width = td->td_imagewidth;
		segment_height = td->td_imagelength - tif->tif_row;
		if (segment_height > td->td_rowsperstrip)
			segment_height = td->td_rowsperstrip;
		sp->bytesperline = TIFFScanlineSize(tif);
	}
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE && s > 0) {
		/* for PC 2, scale down the strip/tile size
		 * to match a downsampled component
		 */
		segment_width = TIFFhowmany(segment_width, sp->h_sampling);
		segment_height = TIFFhowmany(segment_height, sp->v_sampling);
	}
	if (segment_width > 65535 || segment_height > 65535) {
		TIFFError(module, "Strip/tile too large for JPEG");
		return (0);
	}
	sp->cinfo.c.image_width = segment_width;
	sp->cinfo.c.image_height = segment_height;
	downsampled_input = FALSE;
	if (td->td_planarconfig == PLANARCONFIG_CONTIG) {
		sp->cinfo.c.input_components = td->td_samplesperpixel;
		if (sp->photometric == PHOTOMETRIC_YCBCR) {
			if (sp->jpegcolormode == JPEGCOLORMODE_RGB) {
				sp->cinfo.c.in_color_space = JCS_RGB;
			} else {
				sp->cinfo.c.in_color_space = JCS_YCbCr;
				if (sp->h_sampling != 1 || sp->v_sampling != 1)
					downsampled_input = TRUE;
			}
			if (!TIFFjpeg_set_colorspace(sp, JCS_YCbCr))
				return (0);
			/*
			 * Set Y sampling factors;
			 * we assume jpeg_set_colorspace() set the rest to 1
			 */
			sp->cinfo.c.comp_info[0].h_samp_factor = sp->h_sampling;
			sp->cinfo.c.comp_info[0].v_samp_factor = sp->v_sampling;
		} else {
			sp->cinfo.c.in_color_space = JCS_UNKNOWN;
			if (!TIFFjpeg_set_colorspace(sp, JCS_UNKNOWN))
				return (0);
			/* jpeg_set_colorspace set all sampling factors to 1 */
		}
	} else {
		sp->cinfo.c.input_components = 1;
		sp->cinfo.c.in_color_space = JCS_UNKNOWN;
		if (!TIFFjpeg_set_colorspace(sp, JCS_UNKNOWN))
			return (0);
		sp->cinfo.c.comp_info[0].component_id = s;
		/* jpeg_set_colorspace() set sampling factors to 1 */
		if (sp->photometric == PHOTOMETRIC_YCBCR && s > 0) {
			sp->cinfo.c.comp_info[0].quant_tbl_no = 1;
			sp->cinfo.c.comp_info[0].dc_tbl_no = 1;
			sp->cinfo.c.comp_info[0].ac_tbl_no = 1;
		}
	}
	/* ensure libjpeg won't write any extraneous markers */
	sp->cinfo.c.write_JFIF_header = FALSE;
	sp->cinfo.c.write_Adobe_marker = FALSE;
	/* set up table handling correctly */
	if (! (sp->jpegtablesmode & JPEGTABLESMODE_QUANT)) {
		if (!TIFFjpeg_set_quality(sp, sp->jpegquality, FALSE))
			return (0);
		unsuppress_quant_table(sp, 0);
		unsuppress_quant_table(sp, 1);
	}
	if (sp->jpegtablesmode & JPEGTABLESMODE_HUFF)
		sp->cinfo.c.optimize_coding = FALSE;
	else
		sp->cinfo.c.optimize_coding = TRUE;
	if (downsampled_input) {
		/* Need to use raw-data interface to libjpeg */
		sp->cinfo.c.raw_data_in = TRUE;
		tif->tif_encoderow = JPEGEncodeRaw;
		tif->tif_encodestrip = JPEGEncodeRaw;
		tif->tif_encodetile = JPEGEncodeRaw;
	} else {
		/* Use normal interface to libjpeg */
		sp->cinfo.c.raw_data_in = FALSE;
		tif->tif_encoderow = JPEGEncode;
		tif->tif_encodestrip = JPEGEncode;
		tif->tif_encodetile = JPEGEncode;
	}
	/* Start JPEG compressor */
	if (!TIFFjpeg_start_compress(sp, FALSE))
		return (0);
	/* Allocate downsampled-data buffers if needed */
	if (downsampled_input) {
		if (!alloc_downsampled_buffers(tif, sp->cinfo.c.comp_info,
					       sp->cinfo.c.num_components))
			return (0);
	}
	sp->scancount = 0;

	return (1);
}

/*
 * Encode a chunk of pixels.
 * "Standard" case: incoming data is not downsampled.
 */

static int
JPEGEncode(tif, buf, cc, s)
    TIFF* tif;
    tidata_t buf;
    tsize_t cc;
    tsample_t s;
{
	JPEGState *sp = JState(tif);
	tsize_t nrows;
	JSAMPROW bufptr[1];

	(void) s;
	assert(sp != NULL);
	/* data is expected to be supplied in multiples of a scanline */
	nrows = cc / sp->bytesperline;

	while (nrows-- > 0) {
		bufptr[0] = (JSAMPROW) buf;
		if (TIFFjpeg_write_scanlines(sp, bufptr, 1) != 1)
			return (0);
		if (nrows > 0)
			tif->tif_row++;
		buf += sp->bytesperline;
	}
	return (1);
}

/*
 * Encode a chunk of pixels.
 * Incoming data is expected to be downsampled per sampling factors.
 */

static int
JPEGEncodeRaw(tif, buf, cc, s)
    TIFF* tif;
    tidata_t buf;
    tsize_t cc;
    tsample_t s;
{
	JPEGState *sp = JState(tif);
	JSAMPLE* inptr;
	JSAMPLE* outptr;
	tsize_t nrows;
	JDIMENSION clumps_per_line, nclump;
	int clumpoffset, ci, xpos, ypos;
	jpeg_component_info* compptr;
	int samples_per_clump = sp->samplesperclump;

	(void) s;
	assert(sp != NULL);
	/* data is expected to be supplied in multiples of a scanline */
	nrows = cc / sp->bytesperline;

	/* Cb,Cr both have sampling factors 1, so this is correct */
	clumps_per_line = sp->cinfo.c.comp_info[1].downsampled_width;

	while (nrows-- > 0) {
		/*
		 * Fastest way to separate the data is to make one pass
		 * over the scanline for each row of each component.
		 */
		clumpoffset = 0;		/* first sample in clump */
		for (ci = 0, compptr = sp->cinfo.c.comp_info;
		     ci < sp->cinfo.c.num_components;
		     ci++, compptr++) {
		    int hsamp = compptr->h_samp_factor;
		    int vsamp = compptr->v_samp_factor;
		    int padding = (int) (compptr->width_in_blocks * DCTSIZE -
					 clumps_per_line * hsamp);
		    for (ypos = 0; ypos < vsamp; ypos++) {
			inptr = ((JSAMPLE*) buf) + clumpoffset;
			outptr = sp->ds_buffer[ci][sp->scancount*vsamp + ypos];
			if (hsamp == 1) {
			    /* fast path for at least Cb and Cr */
			    for (nclump = clumps_per_line; nclump-- > 0; ) {
				*outptr++ = inptr[0];
				inptr += samples_per_clump;
			    }
			} else {
			    /* general case */
			    for (nclump = clumps_per_line; nclump-- > 0; ) {
				for (xpos = 0; xpos < hsamp; xpos++)
				    *outptr++ = inptr[xpos];
				inptr += samples_per_clump;
			    }
			}
			/* pad each scanline as needed */
			for (xpos = 0; xpos < padding; xpos++) {
			    *outptr = outptr[-1];
			    outptr++;
			}
			clumpoffset += hsamp;
		    }
		}
		sp->scancount++;
		if (sp->scancount >= DCTSIZE) {
			int n = sp->cinfo.c.max_v_samp_factor * DCTSIZE;
			if (TIFFjpeg_write_raw_data(sp, sp->ds_buffer, n) != n)
				return (0);
			sp->scancount = 0;
		}
		if (nrows > 0)
			tif->tif_row++;
		buf += sp->bytesperline;
	}
	return (1);
}

/*
 * Finish up at the end of a strip or tile.
 */

static int
JPEGPostEncode(tif)
    TIFF* tif;
{
	JPEGState *sp = JState(tif);

	if (sp->scancount > 0) {
		/*
		 * Need to emit a partial bufferload of downsampled data.
		 * Pad the data vertically.
		 */
		int ci, ypos, n;
		jpeg_component_info* compptr;

		for (ci = 0, compptr = sp->cinfo.c.comp_info;
		     ci < sp->cinfo.c.num_components;
		     ci++, compptr++) {
			int vsamp = compptr->v_samp_factor;
			tsize_t row_width = compptr->width_in_blocks * DCTSIZE
				* sizeof(JSAMPLE);
			for (ypos = sp->scancount * vsamp;
			     ypos < DCTSIZE * vsamp; ypos++) {
				_TIFFmemcpy((tdata_t)sp->ds_buffer[ci][ypos],
					    (tdata_t)sp->ds_buffer[ci][ypos-1],
					    row_width);

			}
		}
		n = sp->cinfo.c.max_v_samp_factor * DCTSIZE;
		if (TIFFjpeg_write_raw_data(sp, sp->ds_buffer, n) != n)
			return (0);
	}

	return (TIFFjpeg_finish_compress(JState(tif)));
}

static void
JPEGCleanup(tif)
    TIFF* tif;
{
	if (tif->tif_data) {
		JPEGState *sp = JState(tif);
		TIFFjpeg_destroy(sp);		/* release libjpeg resources */
		if (sp->jpegtables) {		/* tag value */
			TkimgTIFFfree(sp->jpegtables);
		}
		TkimgTIFFfree(tif->tif_data);	/* release local state */
		tif->tif_data = NULL;
	}
}


static int
JPEGVSetField(tif, tag, ap)
    TIFF* tif;
    ttag_t tag;
    va_list ap;
{
	JPEGState* sp = JState(tif);
	TIFFDirectory* td = &tif->tif_dir;
	uint32 v32;

	switch (tag) {
	case TIFFTAG_JPEGTABLES:
		v32 = va_arg(ap, uint32);
		if (v32 == 0) {
			/* XXX */
			return (0);
		}
		_TIFFsetByteArray(&sp->jpegtables, va_arg(ap, void*),
		    (long) v32);
		sp->jpegtables_length = v32;
		TIFFSetFieldBit(tif, FIELD_JPEGTABLES);
		break;
	case TIFFTAG_JPEGQUALITY:
		sp->jpegquality = va_arg(ap, int);
		return (1);			/* pseudo tag */
	case TIFFTAG_JPEGCOLORMODE:
		sp->jpegcolormode = va_arg(ap, int);
		/*
		 * Mark whether returned data is up-sampled or not
		 * so TIFFStripSize and TIFFTileSize return values
		 * that reflect the true amount of data.
		 */
		tif->tif_flags &= ~TIFF_UPSAMPLED;
		if (td->td_planarconfig == PLANARCONFIG_CONTIG) {
		    if (td->td_photometric == PHOTOMETRIC_YCBCR &&
		      sp->jpegcolormode == JPEGCOLORMODE_RGB) {
			tif->tif_flags |= TIFF_UPSAMPLED;
		    } else {
			if (td->td_ycbcrsubsampling[0] != 1 ||
			    td->td_ycbcrsubsampling[1] != 1)
			    ; /* XXX what about up-sampling? */
		    }
		}
		/*
		 * Must recalculate cached tile size
		 * in case sampling state changed.
		 */
		tif->tif_tilesize = TIFFTileSize(tif);
		return (1);			/* pseudo tag */
	case TIFFTAG_JPEGTABLESMODE:
		sp->jpegtablesmode = va_arg(ap, int);
		return (1);			/* pseudo tag */
	default:
		return (*sp->vsetparent)(tif, tag, ap);
	}
	tif->tif_flags |= TIFF_DIRTYDIRECT;
	return (1);
}


static int
JPEGVGetField(tif, tag, ap)
    TIFF* tif;
    ttag_t tag;
    va_list ap;
{
	JPEGState* sp = JState(tif);

	switch (tag) {
	case TIFFTAG_JPEGTABLES:
		*va_arg(ap, uint32*) = (uint32) sp->jpegtables_length;
		*va_arg(ap, void**) = sp->jpegtables;
		break;
	case TIFFTAG_JPEGQUALITY:
		*va_arg(ap, int*) = sp->jpegquality;
		break;
	case TIFFTAG_JPEGCOLORMODE:
		*va_arg(ap, int*) = sp->jpegcolormode;
		break;
	case TIFFTAG_JPEGTABLESMODE:
		*va_arg(ap, int*) = sp->jpegtablesmode;
		break;
	default:
		return (*sp->vgetparent)(tif, tag, ap);
	}
	return (1);
}

static uint32
JPEGDefaultStripSize(tif, s)
    TIFF* tif;
    uint32 s;
{
	JPEGState* sp = JState(tif);
	TIFFDirectory *td = &tif->tif_dir;

	s = (*sp->defsparent)(tif, s);
	if (s < td->td_imagelength)
		s = TIFFroundup(s, td->td_ycbcrsubsampling[1] * DCTSIZE);
	return (s);
}

static void
JPEGDefaultTileSize(tif, tw, th)
    TIFF* tif;
    uint32* tw;
    uint32* th;
{
	JPEGState* sp = JState(tif);
	TIFFDirectory *td = &tif->tif_dir;

	(*sp->deftparent)(tif, tw, th);
	*tw = TIFFroundup(*tw, td->td_ycbcrsubsampling[0] * DCTSIZE);
	*th = TIFFroundup(*th, td->td_ycbcrsubsampling[1] * DCTSIZE);
}

int
TkimgTIFFInitJpeg(handle, scheme)
    TIFF* handle;
    int scheme;
{
    TIFF *tif = (TIFF *) handle;
    JPEGState* sp;

    assert(scheme == COMPRESSION_JPEG);

    /*
     * We assume here that package jpegtcl is loaded and its stub
     * table initialized. This happens in tiff.c, just before the
     * codec is registered.
     */

    /*
     * Allocate state block so tag methods have storage to record values.
     */
    tif->tif_data = (tidata_t) TkimgTIFFmalloc(sizeof (JPEGState));
    if (tif->tif_data == NULL) {
      TIFFError("TIFFInitJPEG", "No space for JPEG state block");
      return (0);
    }
    sp = JState(tif);
    sp->tif = tif;				/* back link */

    /*
     * Merge codec-specific tag information and
     * override parent get/set field methods.
     */
    _TIFFMergeFieldInfo(tif, jpegFieldInfo, N(jpegFieldInfo));
    sp->vgetparent = tif->tif_tagmethods.vgetfield;
    tif->tif_tagmethods.vgetfield = JPEGVGetField;	/* hook for codec tags */
    sp->vsetparent = tif->tif_tagmethods.vsetfield;
    tif->tif_tagmethods.vsetfield = JPEGVSetField;	/* hook for codec tags */

    /* Default values for codec-specific fields */
    sp->jpegtables = NULL;
    sp->jpegtables_length = 0;
    sp->jpegquality = 75;			/* Default IJG quality */
    sp->jpegcolormode = JPEGCOLORMODE_RAW;
    sp->jpegtablesmode = JPEGTABLESMODE_QUANT | JPEGTABLESMODE_HUFF;

    /*
     * Install codec methods.
     */
    tif->tif_setupdecode = JPEGSetupDecode;
    tif->tif_predecode = JPEGPreDecode;
    tif->tif_decoderow = JPEGDecode;
    tif->tif_decodestrip = JPEGDecode;
    tif->tif_decodetile = JPEGDecode;
    tif->tif_setupencode = JPEGSetupEncode;
    tif->tif_preencode = JPEGPreEncode;
    tif->tif_postencode = JPEGPostEncode;
    tif->tif_encoderow = JPEGEncode;
    tif->tif_encodestrip = JPEGEncode;
    tif->tif_encodetile = JPEGEncode;
    tif->tif_cleanup = JPEGCleanup;
    sp->defsparent = tif->tif_defstripsize;
    tif->tif_defstripsize = JPEGDefaultStripSize;
    sp->deftparent = tif->tif_deftilesize;
    tif->tif_deftilesize = JPEGDefaultTileSize;
    tif->tif_flags |= TIFF_NOBITREV;	/* no bit reversal, please */

    /*
     * Initialize libjpeg.
     */
    if (tif->tif_mode == O_RDONLY) {
      if (!TIFFjpeg_create_decompress(sp))
	return (0);
    } else {
      if (!TIFFjpeg_create_compress(sp))
	return (0);
    }

    return (1);
}
