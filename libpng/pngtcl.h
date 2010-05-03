/*
 * pngtcl.h --
 *
 *	Interface to libpng.
 *
 * Copyright (c) 2002-2004 Andreas Kupries <andreas_kupries@users.sourceforge.net>
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

#ifndef __PNGTCL_H__
#define __PNGTCL_H__

#include "tcl.h"

#define PNGTCL_MAJOR_VERSION	1
#define PNGTCL_MINOR_VERSION	2
#define PNGTCL_RELEASE_LEVEL	TCL_RELEASE
#define PNGTCL_RELEASE_SERIAL	34

#define PNGTCL_VERSION		"1.4.1"
#define PNGTCL_PATCH_LEVEL	"1.4.1"

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

/*
 * These macros are used to control whether functions are being declared for
 * import or export in Windows,
 * They map to no-op declarations on non-Windows systems.
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
 * storage class will be set to DLLEXPORT.  At the end of the header file, the
 * storage class will be reset to DLLIMPORt.
 */

#ifdef BUILD_pngtcl
# define PNGTCLAPI extern DLLEXPORT
#else
# ifdef USE_PNGTCL_STUBS
#  define PNGTCLAPI extern
# else
#  define PNGTCLAPI extern DLLIMPORT
# endif
#endif

/*
 *----------------------------------------------------------------------------
 * C API for Pngtcl generic layer
 *----------------------------------------------------------------------------
 */

/*
 *----------------------------------------------------------------------------
 * Function prototypes for publically accessible routines
 *----------------------------------------------------------------------------
 */

#include "pngtclDecls.h"

/*
 *----------------------------------------------------------------------------
 * Function prototypes for stub initialization.
 *----------------------------------------------------------------------------
 */

#ifdef USE_PNGTCL_STUBS
const char *
Pngtcl_InitStubs(Tcl_Interp *interp, const char *version, int exact);
#else
/*
 * When not using stubs, make it a macro.
 */

#define Pngtcl_InitStubs(interp, version, exact) \
    Tcl_PkgRequire(interp, "pngtcl", (CONST84 char *) version, exact)
#endif

#undef TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLIMPORT

#endif /* RC_INVOKED */
#endif /* __PNGTCL_H__ */
