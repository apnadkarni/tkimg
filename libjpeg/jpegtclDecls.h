/*
 * jpegtclDecls.h --
 *
 *	Declarations of functions in the platform independent public JPEGTCL API.
 *
 */

#ifndef _JPEGTCLDECLS
#define _JPEGTCLDECLS

/*
 * WARNING: The contents of this file is automatically generated by the
 * genStubs.tcl script. Any modifications to the function declarations
 * below should be made in the jpegtcl.decls script.
 */

#include <tcl.h>

/* undef Tcl macros that conflict with libjpeg stuff (sigh) */
#undef EXTERN

#include "jpegtclDeclsMask.h"
#include "../compat/libjpeg/jpeglib.h"
#include "../compat/libjpeg/jerror.h"
#include "jpegtclDeclsUnmask.h"

/*
 * And recreate the definition of <tcl.h>, we need it below.
 */

#undef EXTERN
#ifdef __cplusplus
#   define EXTERN extern "C" TCL_STORAGE_CLASS
#else
#   define EXTERN extern TCL_STORAGE_CLASS
#endif


/* !BEGIN!: Do not edit below this line. */

/*
 * Exported function declarations:
 */

/* 0 */
EXTERN struct jpeg_error_mgr * jpeg_std_error(struct jpeg_error_mgr *err);
/* 1 */
EXTERN void		jpeg_CreateCompress(j_compress_ptr cinfo,
				int version, size_t structsize);
/* 2 */
EXTERN void		jpeg_CreateDecompress(j_decompress_ptr cinfo,
				int version, size_t structsize);
/* 3 */
EXTERN void		jpeg_destroy_compress(j_compress_ptr cinfo);
/* 4 */
EXTERN void		jpeg_destroy_decompress(j_decompress_ptr cinfo);
/* 5 */
EXTERN void		jpeg_stdio_dest(j_compress_ptr cinfo, FILE *outfile);
/* 6 */
EXTERN void		jpeg_stdio_src(j_decompress_ptr cinfo, FILE *infile);
/* 7 */
EXTERN void		jpeg_set_defaults(j_compress_ptr cinfo);
/* 8 */
EXTERN void		jpeg_set_colorspace(j_compress_ptr cinfo,
				J_COLOR_SPACE colorspace);
/* 9 */
EXTERN void		jpeg_default_colorspace(j_compress_ptr cinfo);
/* 10 */
EXTERN void		jpeg_set_quality(j_compress_ptr cinfo, int quality,
				boolean force_baseline);
/* 11 */
EXTERN void		jpeg_set_linear_quality(j_compress_ptr cinfo,
				int scale_factor, boolean force_baseline);
/* 12 */
EXTERN void		jpeg_add_quant_table(j_compress_ptr cinfo,
				int which_tbl,
				const unsigned int *basic_table,
				int scale_factor, boolean force_baseline);
/* 13 */
EXTERN int		jpeg_quality_scaling(int quality);
/* 14 */
EXTERN void		jpeg_simple_progression(j_compress_ptr cinfo);
/* 15 */
EXTERN void		jpeg_suppress_tables(j_compress_ptr cinfo,
				boolean suppress);
/* 16 */
EXTERN JQUANT_TBL *	jpeg_alloc_quant_table(j_common_ptr cinfo);
/* 17 */
EXTERN JHUFF_TBL *	jpeg_alloc_huff_table(j_common_ptr cinfo);
/* 18 */
EXTERN void		jpeg_start_compress(j_compress_ptr cinfo,
				boolean write_all_tables);
/* 19 */
EXTERN JDIMENSION	jpeg_write_scanlines(j_compress_ptr cinfo,
				JSAMPARRAY scanlines, JDIMENSION num_lines);
/* 20 */
EXTERN void		jpeg_finish_compress(j_compress_ptr cinfo);
/* 21 */
EXTERN JDIMENSION	jpeg_write_raw_data(j_compress_ptr cinfo,
				JSAMPIMAGE data, JDIMENSION num_lines);
/* 22 */
EXTERN void		jpeg_write_marker(j_compress_ptr cinfo, int marker,
				const JOCTET *dataptr, unsigned int datalen);
/* 23 */
EXTERN void		jpeg_write_m_header(j_compress_ptr cinfo, int marker,
				unsigned int datalen);
/* 24 */
EXTERN void		jpeg_write_m_byte(j_compress_ptr cinfo, int val);
/* 25 */
EXTERN void		jpeg_write_tables(j_compress_ptr cinfo);
/* 26 */
EXTERN int		jpeg_read_header(j_decompress_ptr cinfo,
				boolean require_image);
/* 27 */
EXTERN boolean		jpeg_start_decompress(j_decompress_ptr cinfo);
/* 28 */
EXTERN JDIMENSION	jpeg_read_scanlines(j_decompress_ptr cinfo,
				JSAMPARRAY scanlines, JDIMENSION max_lines);
/* 29 */
EXTERN boolean		jpeg_finish_decompress(j_decompress_ptr cinfo);
/* 30 */
EXTERN JDIMENSION	jpeg_read_raw_data(j_decompress_ptr cinfo,
				JSAMPIMAGE data, JDIMENSION max_lines);
/* 31 */
EXTERN boolean		jpeg_has_multiple_scans(j_decompress_ptr cinfo);
/* 32 */
EXTERN boolean		jpeg_start_output(j_decompress_ptr cinfo,
				int scan_number);
/* 33 */
EXTERN boolean		jpeg_finish_output(j_decompress_ptr cinfo);
/* 34 */
EXTERN boolean		jpeg_input_complete(j_decompress_ptr cinfo);
/* 35 */
EXTERN void		jpeg_new_colormap(j_decompress_ptr cinfo);
/* 36 */
EXTERN int		jpeg_consume_input(j_decompress_ptr cinfo);
/* 37 */
EXTERN void		jpeg_calc_output_dimensions(j_decompress_ptr cinfo);
/* 38 */
EXTERN void		jpeg_save_markers(j_decompress_ptr cinfo,
				int marker_code, unsigned int length_limit);
/* 39 */
EXTERN void		jpeg_set_marker_processor(j_decompress_ptr cinfo,
				int marker_code,
				jpeg_marker_parser_method routine);
/* 40 */
EXTERN jvirt_barray_ptr * jpeg_read_coefficients(j_decompress_ptr cinfo);
/* 41 */
EXTERN void		jpeg_write_coefficients(j_compress_ptr cinfo,
				jvirt_barray_ptr *coef_arrays);
/* 42 */
EXTERN void		jpeg_copy_critical_parameters(
				j_decompress_ptr srcinfo,
				j_compress_ptr dstinfo);
/* 43 */
EXTERN void		jpeg_abort_compress(j_compress_ptr cinfo);
/* 44 */
EXTERN void		jpeg_abort_decompress(j_decompress_ptr cinfo);
/* 45 */
EXTERN void		jpeg_abort(j_common_ptr cinfo);
/* 46 */
EXTERN void		jpeg_destroy(j_common_ptr cinfo);
/* 47 */
EXTERN boolean		jpeg_resync_to_restart(j_decompress_ptr cinfo,
				int desired);

typedef struct JpegtclStubs {
    int magic;
    const struct JpegtclStubHooks *hooks;

    struct jpeg_error_mgr * (*jpeg_std_errorPtr) (struct jpeg_error_mgr *err); /* 0 */
    void (*jpeg_CreateCompressPtr) (j_compress_ptr cinfo, int version, size_t structsize); /* 1 */
    void (*jpeg_CreateDecompressPtr) (j_decompress_ptr cinfo, int version, size_t structsize); /* 2 */
    void (*jpeg_destroy_compressPtr) (j_compress_ptr cinfo); /* 3 */
    void (*jpeg_destroy_decompressPtr) (j_decompress_ptr cinfo); /* 4 */
    void (*jpeg_stdio_destPtr) (j_compress_ptr cinfo, FILE *outfile); /* 5 */
    void (*jpeg_stdio_srcPtr) (j_decompress_ptr cinfo, FILE *infile); /* 6 */
    void (*jpeg_set_defaultsPtr) (j_compress_ptr cinfo); /* 7 */
    void (*jpeg_set_colorspacePtr) (j_compress_ptr cinfo, J_COLOR_SPACE colorspace); /* 8 */
    void (*jpeg_default_colorspacePtr) (j_compress_ptr cinfo); /* 9 */
    void (*jpeg_set_qualityPtr) (j_compress_ptr cinfo, int quality, boolean force_baseline); /* 10 */
    void (*jpeg_set_linear_qualityPtr) (j_compress_ptr cinfo, int scale_factor, boolean force_baseline); /* 11 */
    void (*jpeg_add_quant_tablePtr) (j_compress_ptr cinfo, int which_tbl, const unsigned int *basic_table, int scale_factor, boolean force_baseline); /* 12 */
    int (*jpeg_quality_scalingPtr) (int quality); /* 13 */
    void (*jpeg_simple_progressionPtr) (j_compress_ptr cinfo); /* 14 */
    void (*jpeg_suppress_tablesPtr) (j_compress_ptr cinfo, boolean suppress); /* 15 */
    JQUANT_TBL * (*jpeg_alloc_quant_tablePtr) (j_common_ptr cinfo); /* 16 */
    JHUFF_TBL * (*jpeg_alloc_huff_tablePtr) (j_common_ptr cinfo); /* 17 */
    void (*jpeg_start_compressPtr) (j_compress_ptr cinfo, boolean write_all_tables); /* 18 */
    JDIMENSION (*jpeg_write_scanlinesPtr) (j_compress_ptr cinfo, JSAMPARRAY scanlines, JDIMENSION num_lines); /* 19 */
    void (*jpeg_finish_compressPtr) (j_compress_ptr cinfo); /* 20 */
    JDIMENSION (*jpeg_write_raw_dataPtr) (j_compress_ptr cinfo, JSAMPIMAGE data, JDIMENSION num_lines); /* 21 */
    void (*jpeg_write_markerPtr) (j_compress_ptr cinfo, int marker, const JOCTET *dataptr, unsigned int datalen); /* 22 */
    void (*jpeg_write_m_headerPtr) (j_compress_ptr cinfo, int marker, unsigned int datalen); /* 23 */
    void (*jpeg_write_m_bytePtr) (j_compress_ptr cinfo, int val); /* 24 */
    void (*jpeg_write_tablesPtr) (j_compress_ptr cinfo); /* 25 */
    int (*jpeg_read_headerPtr) (j_decompress_ptr cinfo, boolean require_image); /* 26 */
    boolean (*jpeg_start_decompressPtr) (j_decompress_ptr cinfo); /* 27 */
    JDIMENSION (*jpeg_read_scanlinesPtr) (j_decompress_ptr cinfo, JSAMPARRAY scanlines, JDIMENSION max_lines); /* 28 */
    boolean (*jpeg_finish_decompressPtr) (j_decompress_ptr cinfo); /* 29 */
    JDIMENSION (*jpeg_read_raw_dataPtr) (j_decompress_ptr cinfo, JSAMPIMAGE data, JDIMENSION max_lines); /* 30 */
    boolean (*jpeg_has_multiple_scansPtr) (j_decompress_ptr cinfo); /* 31 */
    boolean (*jpeg_start_outputPtr) (j_decompress_ptr cinfo, int scan_number); /* 32 */
    boolean (*jpeg_finish_outputPtr) (j_decompress_ptr cinfo); /* 33 */
    boolean (*jpeg_input_completePtr) (j_decompress_ptr cinfo); /* 34 */
    void (*jpeg_new_colormapPtr) (j_decompress_ptr cinfo); /* 35 */
    int (*jpeg_consume_inputPtr) (j_decompress_ptr cinfo); /* 36 */
    void (*jpeg_calc_output_dimensionsPtr) (j_decompress_ptr cinfo); /* 37 */
    void (*jpeg_save_markersPtr) (j_decompress_ptr cinfo, int marker_code, unsigned int length_limit); /* 38 */
    void (*jpeg_set_marker_processorPtr) (j_decompress_ptr cinfo, int marker_code, jpeg_marker_parser_method routine); /* 39 */
    jvirt_barray_ptr * (*jpeg_read_coefficientsPtr) (j_decompress_ptr cinfo); /* 40 */
    void (*jpeg_write_coefficientsPtr) (j_compress_ptr cinfo, jvirt_barray_ptr *coef_arrays); /* 41 */
    void (*jpeg_copy_critical_parametersPtr) (j_decompress_ptr srcinfo, j_compress_ptr dstinfo); /* 42 */
    void (*jpeg_abort_compressPtr) (j_compress_ptr cinfo); /* 43 */
    void (*jpeg_abort_decompressPtr) (j_decompress_ptr cinfo); /* 44 */
    void (*jpeg_abortPtr) (j_common_ptr cinfo); /* 45 */
    void (*jpeg_destroyPtr) (j_common_ptr cinfo); /* 46 */
    boolean (*jpeg_resync_to_restartPtr) (j_decompress_ptr cinfo, int desired); /* 47 */
} JpegtclStubs;

/*
 * Used to tag functions that are only to be visible within the module being
 * built and not outside it (where this is supported by the linker).
 */

#ifndef MODULE_SCOPE
#   ifdef __cplusplus
#	define MODULE_SCOPE extern "C"
#   else
#	define MODULE_SCOPE extern
#   endif
#endif

MODULE_SCOPE const JpegtclStubs *jpegtclStubsPtr;

#if defined(USE_JPEGTCL_STUBS) && !defined(USE_JPEGTCL_STUB_PROCS)

/*
 * Inline function declarations:
 */

#ifndef jpeg_std_error
#define jpeg_std_error \
	(jpegtclStubsPtr->jpeg_std_errorPtr) /* 0 */
#endif
#ifndef jpeg_CreateCompress
#define jpeg_CreateCompress \
	(jpegtclStubsPtr->jpeg_CreateCompressPtr) /* 1 */
#endif
#ifndef jpeg_CreateDecompress
#define jpeg_CreateDecompress \
	(jpegtclStubsPtr->jpeg_CreateDecompressPtr) /* 2 */
#endif
#ifndef jpeg_destroy_compress
#define jpeg_destroy_compress \
	(jpegtclStubsPtr->jpeg_destroy_compressPtr) /* 3 */
#endif
#ifndef jpeg_destroy_decompress
#define jpeg_destroy_decompress \
	(jpegtclStubsPtr->jpeg_destroy_decompressPtr) /* 4 */
#endif
#ifndef jpeg_stdio_dest
#define jpeg_stdio_dest \
	(jpegtclStubsPtr->jpeg_stdio_destPtr) /* 5 */
#endif
#ifndef jpeg_stdio_src
#define jpeg_stdio_src \
	(jpegtclStubsPtr->jpeg_stdio_srcPtr) /* 6 */
#endif
#ifndef jpeg_set_defaults
#define jpeg_set_defaults \
	(jpegtclStubsPtr->jpeg_set_defaultsPtr) /* 7 */
#endif
#ifndef jpeg_set_colorspace
#define jpeg_set_colorspace \
	(jpegtclStubsPtr->jpeg_set_colorspacePtr) /* 8 */
#endif
#ifndef jpeg_default_colorspace
#define jpeg_default_colorspace \
	(jpegtclStubsPtr->jpeg_default_colorspacePtr) /* 9 */
#endif
#ifndef jpeg_set_quality
#define jpeg_set_quality \
	(jpegtclStubsPtr->jpeg_set_qualityPtr) /* 10 */
#endif
#ifndef jpeg_set_linear_quality
#define jpeg_set_linear_quality \
	(jpegtclStubsPtr->jpeg_set_linear_qualityPtr) /* 11 */
#endif
#ifndef jpeg_add_quant_table
#define jpeg_add_quant_table \
	(jpegtclStubsPtr->jpeg_add_quant_tablePtr) /* 12 */
#endif
#ifndef jpeg_quality_scaling
#define jpeg_quality_scaling \
	(jpegtclStubsPtr->jpeg_quality_scalingPtr) /* 13 */
#endif
#ifndef jpeg_simple_progression
#define jpeg_simple_progression \
	(jpegtclStubsPtr->jpeg_simple_progressionPtr) /* 14 */
#endif
#ifndef jpeg_suppress_tables
#define jpeg_suppress_tables \
	(jpegtclStubsPtr->jpeg_suppress_tablesPtr) /* 15 */
#endif
#ifndef jpeg_alloc_quant_table
#define jpeg_alloc_quant_table \
	(jpegtclStubsPtr->jpeg_alloc_quant_tablePtr) /* 16 */
#endif
#ifndef jpeg_alloc_huff_table
#define jpeg_alloc_huff_table \
	(jpegtclStubsPtr->jpeg_alloc_huff_tablePtr) /* 17 */
#endif
#ifndef jpeg_start_compress
#define jpeg_start_compress \
	(jpegtclStubsPtr->jpeg_start_compressPtr) /* 18 */
#endif
#ifndef jpeg_write_scanlines
#define jpeg_write_scanlines \
	(jpegtclStubsPtr->jpeg_write_scanlinesPtr) /* 19 */
#endif
#ifndef jpeg_finish_compress
#define jpeg_finish_compress \
	(jpegtclStubsPtr->jpeg_finish_compressPtr) /* 20 */
#endif
#ifndef jpeg_write_raw_data
#define jpeg_write_raw_data \
	(jpegtclStubsPtr->jpeg_write_raw_dataPtr) /* 21 */
#endif
#ifndef jpeg_write_marker
#define jpeg_write_marker \
	(jpegtclStubsPtr->jpeg_write_markerPtr) /* 22 */
#endif
#ifndef jpeg_write_m_header
#define jpeg_write_m_header \
	(jpegtclStubsPtr->jpeg_write_m_headerPtr) /* 23 */
#endif
#ifndef jpeg_write_m_byte
#define jpeg_write_m_byte \
	(jpegtclStubsPtr->jpeg_write_m_bytePtr) /* 24 */
#endif
#ifndef jpeg_write_tables
#define jpeg_write_tables \
	(jpegtclStubsPtr->jpeg_write_tablesPtr) /* 25 */
#endif
#ifndef jpeg_read_header
#define jpeg_read_header \
	(jpegtclStubsPtr->jpeg_read_headerPtr) /* 26 */
#endif
#ifndef jpeg_start_decompress
#define jpeg_start_decompress \
	(jpegtclStubsPtr->jpeg_start_decompressPtr) /* 27 */
#endif
#ifndef jpeg_read_scanlines
#define jpeg_read_scanlines \
	(jpegtclStubsPtr->jpeg_read_scanlinesPtr) /* 28 */
#endif
#ifndef jpeg_finish_decompress
#define jpeg_finish_decompress \
	(jpegtclStubsPtr->jpeg_finish_decompressPtr) /* 29 */
#endif
#ifndef jpeg_read_raw_data
#define jpeg_read_raw_data \
	(jpegtclStubsPtr->jpeg_read_raw_dataPtr) /* 30 */
#endif
#ifndef jpeg_has_multiple_scans
#define jpeg_has_multiple_scans \
	(jpegtclStubsPtr->jpeg_has_multiple_scansPtr) /* 31 */
#endif
#ifndef jpeg_start_output
#define jpeg_start_output \
	(jpegtclStubsPtr->jpeg_start_outputPtr) /* 32 */
#endif
#ifndef jpeg_finish_output
#define jpeg_finish_output \
	(jpegtclStubsPtr->jpeg_finish_outputPtr) /* 33 */
#endif
#ifndef jpeg_input_complete
#define jpeg_input_complete \
	(jpegtclStubsPtr->jpeg_input_completePtr) /* 34 */
#endif
#ifndef jpeg_new_colormap
#define jpeg_new_colormap \
	(jpegtclStubsPtr->jpeg_new_colormapPtr) /* 35 */
#endif
#ifndef jpeg_consume_input
#define jpeg_consume_input \
	(jpegtclStubsPtr->jpeg_consume_inputPtr) /* 36 */
#endif
#ifndef jpeg_calc_output_dimensions
#define jpeg_calc_output_dimensions \
	(jpegtclStubsPtr->jpeg_calc_output_dimensionsPtr) /* 37 */
#endif
#ifndef jpeg_save_markers
#define jpeg_save_markers \
	(jpegtclStubsPtr->jpeg_save_markersPtr) /* 38 */
#endif
#ifndef jpeg_set_marker_processor
#define jpeg_set_marker_processor \
	(jpegtclStubsPtr->jpeg_set_marker_processorPtr) /* 39 */
#endif
#ifndef jpeg_read_coefficients
#define jpeg_read_coefficients \
	(jpegtclStubsPtr->jpeg_read_coefficientsPtr) /* 40 */
#endif
#ifndef jpeg_write_coefficients
#define jpeg_write_coefficients \
	(jpegtclStubsPtr->jpeg_write_coefficientsPtr) /* 41 */
#endif
#ifndef jpeg_copy_critical_parameters
#define jpeg_copy_critical_parameters \
	(jpegtclStubsPtr->jpeg_copy_critical_parametersPtr) /* 42 */
#endif
#ifndef jpeg_abort_compress
#define jpeg_abort_compress \
	(jpegtclStubsPtr->jpeg_abort_compressPtr) /* 43 */
#endif
#ifndef jpeg_abort_decompress
#define jpeg_abort_decompress \
	(jpegtclStubsPtr->jpeg_abort_decompressPtr) /* 44 */
#endif
#ifndef jpeg_abort
#define jpeg_abort \
	(jpegtclStubsPtr->jpeg_abortPtr) /* 45 */
#endif
#ifndef jpeg_destroy
#define jpeg_destroy \
	(jpegtclStubsPtr->jpeg_destroyPtr) /* 46 */
#endif
#ifndef jpeg_resync_to_restart
#define jpeg_resync_to_restart \
	(jpegtclStubsPtr->jpeg_resync_to_restartPtr) /* 47 */
#endif

#endif /* defined(USE_JPEGTCL_STUBS) && !defined(USE_JPEGTCL_STUB_PROCS) */

/* !END!: Do not edit above this line. */

#endif /* _JPEGTCLDECLS */

