/*
 * tkimg.h --
 *
 *  Interface to tkimg Base package.
 *
 * Copyright (c) 2002 Andreas Kupries <andreas_kupries@users.sourceforge.net>
 *
 * Zveno Pty Ltd makes this software and associated documentation
 * available free of charge for any purpose.  You may make copies
 * of the software but you must include all of this notice on any copy.
 *
 * Zveno Pty Ltd does not warrant that this software is error free
 * or fit for any purpose.  Zveno Pty Ltd disclaims any liability for
 * all claims, expenses, losses, damages and costs any user may incur
 * as a result of using, copying or modifying the software.
 *
 * $Id$
 *
 */

#ifndef __TKIMG_H__
#define __TKIMG_H__

#ifdef _MSC_VER
#pragma warning(disable:4244) /* '=' : conversion from '__int64' to 'int', possible loss of data */
#pragma warning(disable:4761) /* integral size mismatch in argument; conversion supplied */
#endif

#include <stdio.h> /* stdout, and other definitions */
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tk.h"

/*
 * On a few systems, type boolean and/or its values FALSE, TRUE may appear
 * in standard header files.  Or you may have conflicts with application-
 * specific header files that you want to include together with these files.
 * Defining HAVE_BOOLEAN before including tkimg.h should make it work.
 */

/* On windows use the boolean definition from its headers to prevent
 * any conflicts should a user of this header use "windows.h". Without
 * this we will have/get conflicting definitions of 'boolean' ('int'
 * here, 'unsigned' char for windows)
 */

#ifndef HAVE_BOOLEAN
#define HAVE_BOOLEAN
#   ifndef __RPCNDR_H__     /* don't conflict if rpcndr.h already read */
#if defined(_WINDOWS) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_Windows)
typedef unsigned char boolean;
#else
typedef int boolean;
#endif
#endif
#endif

/*
 * Used to block the rest of this header file from resource compilers so
 * we can just get the version info.
 */
#ifndef RC_INVOKED

/* TIP 27 update. If CONST84 is not defined we are compiling against a
 * core before 8.4 and have to disable some CONST'ness.
 */

#ifndef CONST84
#   define CONST84
#endif
#ifndef CONST86
#   define CONST86
#endif

#ifndef TK_PHOTO_COMPOSITE_OVERLAY
#   define TK_PHOTO_COMPOSITE_OVERLAY 0
#endif
#ifndef TK_PHOTO_COMPOSITE_SET
#   define TK_PHOTO_COMPOSITE_SET 1
#endif

/*
 * These macros are used to control whether functions are being declared for
 * import or export in Windows,
 * Assumes that tcl.h defines DLLEXPORT & DLLIMPORT correctly.
 * The default build on windows is for a DLL, which causes the DLLIMPORT
 * and DLLEXPORT macros to be nonempty. To build a static library, the
 * macro STATIC_BUILD should be defined before the inclusion of tcl.h
 *
 * If a function is being declared while it is being built
 * to be included in a shared library, then it should have the DLLEXPORT
 * storage class.  If is being declared for use by a module that is going to
 * link against the shared library, then it should have the DLLIMPORT storage
 * class.  If the symbol is being declared for a static build or for use from a
 * stub library, then the storage class should be empty.
 *
 * The convention is that a macro called BUILD_xxxx, where xxxx is the
 * name of a library we are building, is set on the compile line for sources
 * that are to be placed in the library.  When this macro is set, the
 * TKIMGAPI macro will be set to "extern DLLEXPORT".
 */

#ifdef BUILD_tkimg
# define TKIMGAPI extern DLLEXPORT
#else
# ifdef USE_TKIMG_STUBS
#  define TKIMGAPI extern
# else
#  define TKIMGAPI extern DLLIMPORT
# endif
#endif


/*
 *----------------------------------------------------------------------------
 * Function prototypes for publically accessible routines
 *----------------------------------------------------------------------------
 */

#include "tkimgDecls.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 *----------------------------------------------------------------------------
 * C API for Tkimg generic layer
 *----------------------------------------------------------------------------
 */

#define IMG_SPECIAL (1<<8)
#define IMG_PAD     (IMG_SPECIAL+1)
#define IMG_SPACE   (IMG_SPECIAL+2)
#define IMG_BAD     (IMG_SPECIAL+3)
#define IMG_DONE    (IMG_SPECIAL+4)
#define IMG_CHAN    (IMG_SPECIAL+5)
#define IMG_STRING  (IMG_SPECIAL+6)

/*
 * The variable "tkimg_initialized" contains flags indicating which
 * version of Tcl or Perl we are running:
 *
 *  IMG_TCL    Tcl
 *  IMG_PERL   perl
 *  IMG_COMPOSITE Tcl 8.4 or higher
 *  IMG_NOPANIC Tcl 8.5 or higher
 *
 * These flags will be determined at runtime (except the IMG_PERL
 * flag, for now), so we can use the same dynamic library for all
 * Tcl/Tk versions (and for Perl/Tk in the future).
 */

MODULE_SCOPE int tkimg_initialized;

#define IMG_TCL (1<<9)
#define IMG_PERL (1<<11)
#define IMG_COMPOSITE (1<<14)
#define IMG_NOPANIC (1<<15)

MODULE_SCOPE int TkimgInitUtilities(Tcl_Interp* interp);

/*
 *----------------------------------------------------------------------------
 * Function prototypes for stub initialization.
 *----------------------------------------------------------------------------
 */

#ifdef USE_TKIMG_STUBS
const char *
Tkimg_InitStubs(Tcl_Interp *interp, const char *version, int exact);
#else
/*
 * When not using stubs, make it a macro.
 */

#define Tkimg_InitStubs(interp, version, exact) \
    Tcl_PkgRequire(interp, "tkimg", (CONST84 char *) version, exact)
#endif

#endif /* RC_INVOKED */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TKIMG_H__ */
