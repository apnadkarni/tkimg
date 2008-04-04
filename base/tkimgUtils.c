/*
 *	tkimgMap.tcl
 */

#include <string.h>
#include <stdlib.h>
#include <tcl.h>
#include "tkimg.h"

/*
 * The variable "tkimg_initialized" contains flags indicating which
 * version of Tcl or Perl we are running:
 *
 *      IMG_PERL	perl
 *	IMG_TCL		Tcl
 *	IMG_OBJS	using (Tcl_Obj *) in stead of (char *)
 *	IMG_UTF		Tcl supports UTF-8
 *	IMG_NEWPHOTO	Photo image type proc signatures are 8.3 or higher.
 *	IMG_COMPOSITE	Photo image type proc signatures are 8.4 or higher.
 *
 * These flags will be determined at runtime (except the IMG_PERL
 * flag, for now), so we can use the same dynamic library for all
 * Tcl/Tk versions (and for Perl/Tk in the future).
 *
 * The existence of the CPP macro _LANG implies usage in Perl/Tk.
 *
 * Img 1.2: Support for Tcl 7.6 is dropped. This implies that IMG_OBJS
 *          is always set. Therefore this flag is dropped as well, and
 *          also all code for !IMG_OBJS.
 */

int tkimg_initialized = 0;

int
TkimgInitUtilities(interp)
    Tcl_Interp *interp;
{
#ifdef _LANG
    tkimg_initialized = IMG_PERL|IMG_NEWPHOTO;
#else

    int major, minor, patchlevel, type;
    tkimg_initialized = IMG_TCL;

    Tcl_GetVersion(&major, &minor, &patchlevel, &type);

    if ((major > 8) || ((major == 8) && (minor > 0))) {
	tkimg_initialized |= IMG_UTF;
    }
    if ((major > 8) || ((major == 8) && (minor > 2))) {
	tkimg_initialized |= IMG_NEWPHOTO;
    } else {
	/* If our Tcl version is lower than 8.2, then it still might be
	 * that it supports the patch known as "Img-patch". So, check
	 * for this and if it succeeds then set IMG_NEWPHOTO as well
	 */
	Tcl_CmdInfo cmdInfo;
	if (!Tcl_GetCommandInfo(interp,"image", &cmdInfo)) {
		    Tcl_AppendResult(interp, "cannot find the \"image\" command",
			(char *) NULL);
		tkimg_initialized = 0;
		return TCL_ERROR;
	}
	if (cmdInfo.isNativeObjectProc == 1) {
	    tkimg_initialized |= IMG_NEWPHOTO;
	}
    }
    if ((major > 8) || ((major == 8) && (minor > 3))) {
	tkimg_initialized |= IMG_COMPOSITE;
    }
    if ((major > 8) || ((major == 8) && (minor > 4))) {
	tkimg_initialized |= IMG_NOPANIC;
    }

#endif
    return tkimg_initialized;
}


/*
 *----------------------------------------------------------------------
 *
 * tkimg_GetStringFromObj --
 *
 *	Returns the string representation's byte array pointer and length
 *	for an object.
 *
 * Results:
 *	Returns a pointer to the string representation of objPtr.  If
 *	lengthPtr isn't NULL, the length of the string representation is
 *	stored at *lengthPtr. The byte array referenced by the returned
 *	pointer must not be modified by the caller. Furthermore, the
 *	caller must copy the bytes if they need to retain them since the
 *	object's string rep can change as a result of other operations.
 *      REMARK: This function reacts a little bit different than
 *	Tcl_GetStringFromObj():
 *	- objPtr is allowed to be NULL. In that case the NULL pointer
 *	  will be returned, and the length will be reported to be 0;
 *	In the tkimg_ code there is never a distinction between en empty
 *	string and a NULL pointer, while the latter is easier to check
 *	for. That's the reason for this difference.
 *
 * Side effects:
 *	May call the object's updateStringProc to update the string
 *	representation from the internal representation.
 *
 *----------------------------------------------------------------------
 */

char *
tkimg_GetStringFromObj(objPtr, lengthPtr)
    register Tcl_Obj *objPtr;	/* Object whose string rep byte pointer
				 * should be returned, or NULL */
    register int *lengthPtr;	/* If non-NULL, the location where the
				 * string rep's byte array length should be
				 * stored. If NULL, no length is stored. */
{
    if (!objPtr) {
	if (lengthPtr != NULL) {
	    *lengthPtr = 0;
	}
	return (char *) NULL;
    }
#ifdef _LANG
    {
	char *string = LangString((Arg) objPtr);
	if (lengthPtr != NULL) {
	    *lengthPtr = string ? strlen(string) : 0;
	}
	return string;
    }
#else /* _LANG */
    if (tkimg_initialized & IMG_NEWPHOTO) {
	return Tcl_GetStringFromObj(objPtr, lengthPtr);
    } else {
	char *string =  (char *) objPtr;
	if (lengthPtr != NULL) {
	    *lengthPtr = string ? strlen(string) : 0;
	}
	return string;
    }
#endif /* _LANG */
}


/*
 *----------------------------------------------------------------------
 *
 * tkimg_GetByteArrayFromObj --
 *
 *	Returns the binary representation and length
 *	for a byte array object.
 *
 * Results:
 *	Returns a pointer to the byte representation of objPtr.  If
 *	lengthPtr isn't NULL, the length of the string representation is
 *	stored at *lengthPtr. The byte array referenced by the returned
 *	pointer must not be modified by the caller. Furthermore, the
 *	caller must copy the bytes if they need to retain them since the
 *	object's representation can change as a result of other operations.
 *
 * Side effects:
 *	May call the object's updateStringProc to update the string
 *	representation from the internal representation.
 *
 *----------------------------------------------------------------------
 */
unsigned char *
tkimg_GetByteArrayFromObj(objPtr, lengthPtr)
    register Tcl_Obj *objPtr;	/* Object whose string rep byte pointer
				 * should be returned, or NULL */
    register int *lengthPtr;	/* If non-NULL, the location where the
				 * string rep's byte array length should be
				 * stored. If NULL, no length is stored. */
{
#ifdef _LANG
    char *string = LangString((Arg) objPtr);
    if (lengthPtr != NULL) {
	*lengthPtr = string ? strlen(string) : 0;
    }
    return (unsigned char *) string;
#else /* _LANG */

    if (tkimg_initialized & IMG_NEWPHOTO) {
	return Tcl_GetByteArrayFromObj(objPtr, lengthPtr);
    } else {
	char *string =  (char *) objPtr;
	if (lengthPtr != NULL) {
	    *lengthPtr = string ? strlen(string) : 0;
	}
	return (unsigned char *) string;
    }
#endif /* _LANG */
}


/*
 *----------------------------------------------------------------------
 *
 * tkimg_ListObjGetElements --
 *
 *	Splits an object into its compoments.
 *
 * Results:
 *	If objPtr is a valid list (or can be converted to one),
 *	TCL_OK will be returned. The object will be split in
 *	its components.
 *	Otherwise TCL_ERROR is returned. If interp is not a NULL
 *	pointer, an error message will be left in it as well.
 *
 * Side effects:
 *	May call the object's updateStringProc to update the string
 *	representation from the internal representation.
 *
 *----------------------------------------------------------------------
 */

int
tkimg_ListObjGetElements(interp, objPtr, objc, objv)
    Tcl_Interp *interp;
    Tcl_Obj *objPtr;
    int *objc;
    Tcl_Obj ***objv;
{
#ifndef _LANG
    static Tcl_Obj *staticObj = (Tcl_Obj *) NULL;
#endif
    if (objPtr == NULL) {
	*objc = 0;
	return TCL_OK;
    }
#ifndef _LANG
    if (!(tkimg_initialized & IMG_NEWPHOTO)) {
	if (staticObj != (Tcl_Obj *) NULL) {
	    Tcl_DecrRefCount(staticObj);
	}
	objPtr = staticObj = Tcl_NewStringObj((char *) objPtr, -1);
	Tcl_IncrRefCount(staticObj);
    }
#endif
    return Tcl_ListObjGetElements(interp, objPtr, objc, objv);
}

/*
 *----------------------------------------------------------------------
 *
 * tkimg_Fix*Proc --
 *
 *	Various Compatibility functions
 *
 * Results:
 *	The functions below allow an image type to distinguish
 *	between a call made by Tk 8.3.2 or earlier (4 arguments), versus 8.3
 *	or later (3 arguments) and adapt at runtime. This adaption is done
 *	by shuffling the incoming arguments around to their correct positions.
 *
 * Side effects:
 *	S.a.
 *
 *----------------------------------------------------------------------
 */

void
tkimg_FixChanMatchProc(interp, chan, file, format, width, height)
    Tcl_Interp **interp;
    Tcl_Channel *chan;
    CONST char **file;
    Tcl_Obj **format;
    int **width;
    int **height;
{
    Tcl_Interp *tmp;

    if (tkimg_initialized & IMG_PERL) {
	return;
    }
    if (tkimg_initialized & IMG_NEWPHOTO) {
        tmp = (Tcl_Interp *) *height;
    } else {
        /* Old-style call signature */
        tmp = (Tcl_Interp *) NULL;
    }

    *height = *width;
    *width = (int *) *format;
    *format = (Tcl_Obj *) *file;
    *file = (CONST char *) *chan;
    *chan = (Tcl_Channel) *interp;
    *interp = tmp;
}

void
tkimg_FixObjMatchProc(interp, data, format, width, height)
    Tcl_Interp **interp;
    Tcl_Obj **data;
    Tcl_Obj **format;
    int **width;
    int **height;
{
    Tcl_Interp *tmp;

    if (tkimg_initialized & IMG_PERL) {
	return;
    }
    if (tkimg_initialized & IMG_NEWPHOTO) {
        tmp = (Tcl_Interp *) *height;
    } else {
        /* Old-style call signature */
        tmp = (Tcl_Interp *) NULL;
    }

    *height = *width;
    *width = (int *) *format;
    *format = (Tcl_Obj *) *data;
    *data = (Tcl_Obj *) *interp;
    *interp = tmp;
}

void
tkimg_FixStringWriteProc(data, interp, dataPtr, format, blockPtr)
    Tcl_DString *data;
    Tcl_Interp **interp;
    Tcl_DString **dataPtr;
    Tcl_Obj **format;
    Tk_PhotoImageBlock **blockPtr;
{
    if (!*blockPtr) {
	*blockPtr = (Tk_PhotoImageBlock *) *format;	
	*format   = (Tcl_Obj *) *dataPtr;
	*dataPtr  = data;
	Tcl_DStringInit(data);
    }
}
