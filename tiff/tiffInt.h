/*
 * tiffInit.h --
 */

#include "tifftcl.h"

/*
 * Declarations shared between the .c files of the TIFF format handler.
 */

extern int TkimgTIFFInitZip(TIFF *, int);
extern int TkimgTIFFInitJpeg(TIFF *, int);
extern int TkimgTIFFInitPixar(TIFF *, int);

extern void TkimgTIFFfree(tdata_t data);
extern tdata_t TkimgTIFFmalloc(tsize_t size);
extern tdata_t TkimgTIFFrealloc(tdata_t data, tsize_t size);

#ifndef CONST84
#   define CONST84
#endif
#ifndef CONST86
#   define CONST86
#endif
