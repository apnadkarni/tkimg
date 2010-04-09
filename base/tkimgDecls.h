/*
 * tkimgDecls.h --
 *
 *  Declarations of functions in the platform independent public TKIMG API.
 *
 */

#ifndef _TKIMGDECLS
#define _TKIMGDECLS

/*
 * WARNING: The contents of this file is automatically generated by the
 * genStubs.tcl script. Any modifications to the function declarations
 * below should be made in the tkimg.decls script.
 */

#include "tk.h"

typedef struct tkimg_MFile {
	Tcl_DString *buffer; /* pointer to dynamical string */
	char *data; /* mmencoded source string */
	int c; /* bits left over from previous char */
	int state; /* decoder state (0-4 or IMG_DONE) */
	int length; /* length of physical line already written */
} tkimg_MFile;

/* !BEGIN!: Do not edit below this line. */

/*
 * Exported function declarations:
 */

/* 0 */
EXTERN Tcl_Channel	tkimg_OpenFileChannel(Tcl_Interp *interp,
				const char *fileName, int permissions);
/* 1 */
EXTERN int		tkimg_ReadInit(Tcl_Obj *data, int c,
				tkimg_MFile *handle);
/* 2 */
EXTERN void		tkimg_WriteInit(Tcl_DString *buffer,
				tkimg_MFile *handle);
/* 3 */
EXTERN int		tkimg_Getc(tkimg_MFile *handle);
/* 4 */
EXTERN int		tkimg_Read(tkimg_MFile *handle, char *dst, int count);
/* 5 */
EXTERN int		tkimg_Putc(int c, tkimg_MFile *handle);
/* 6 */
EXTERN int		tkimg_Write(tkimg_MFile *handle, const char *src,
				int count);
/* 7 */
EXTERN void		tkimg_ReadBuffer(int onOff);
/* Slot 8 is reserved */
/* Slot 9 is reserved */
/* 10 */
EXTERN int		tkimg_PhotoPutBlock(Tcl_Interp *interp,
				Tk_PhotoHandle handle,
				Tk_PhotoImageBlock *blockPtr, int x, int y,
				int width, int height, int flags);
/* 11 */
EXTERN int		tkimg_PhotoExpand(Tcl_Interp *interp,
				Tk_PhotoHandle handle, int width, int height);
/* 12 */
EXTERN int		tkimg_PhotoSetSize(Tcl_Interp *interp,
				Tk_PhotoHandle handle, int width, int height);
/* Slot 13 is reserved */
/* Slot 14 is reserved */
/* Slot 15 is reserved */
/* Slot 16 is reserved */
/* Slot 17 is reserved */
/* Slot 18 is reserved */
/* Slot 19 is reserved */
/* 20 */
EXTERN void		tkimg_FixChanMatchProc(Tcl_Interp **interp,
				Tcl_Channel *chan, const char **file,
				Tcl_Obj **format, int **width, int **height);
/* 21 */
EXTERN void		tkimg_FixObjMatchProc(Tcl_Interp **interp,
				Tcl_Obj **data, Tcl_Obj **format,
				int **width, int **height);
/* 22 */
EXTERN void		tkimg_FixStringWriteProc(Tcl_DString *data,
				Tcl_Interp **interp, Tcl_DString **dataPtr,
				Tcl_Obj **format,
				Tk_PhotoImageBlock **blockPtr);
/* Slot 23 is reserved */
/* Slot 24 is reserved */
/* Slot 25 is reserved */
/* Slot 26 is reserved */
/* Slot 27 is reserved */
/* Slot 28 is reserved */
/* Slot 29 is reserved */
/* 30 */
EXTERN const char *	tkimg_GetStringFromObj(Tcl_Obj *objPtr,
				int *lengthPtr);
/* 31 */
EXTERN unsigned char *	tkimg_GetByteArrayFromObj(Tcl_Obj *objPtr,
				int *lengthPtr);
/* 32 */
EXTERN int		tkimg_ListObjGetElements(Tcl_Interp *interp,
				Tcl_Obj *objPtr, int *argc, Tcl_Obj ***argv);

typedef struct TkimgStubs {
    int magic;
    const struct TkimgStubHooks *hooks;

    Tcl_Channel (*tkimg_OpenFileChannelPtr) (Tcl_Interp *interp, const char *fileName, int permissions); /* 0 */
    int (*tkimg_ReadInitPtr) (Tcl_Obj *data, int c, tkimg_MFile *handle); /* 1 */
    void (*tkimg_WriteInitPtr) (Tcl_DString *buffer, tkimg_MFile *handle); /* 2 */
    int (*tkimg_GetcPtr) (tkimg_MFile *handle); /* 3 */
    int (*tkimg_ReadPtr) (tkimg_MFile *handle, char *dst, int count); /* 4 */
    int (*tkimg_PutcPtr) (int c, tkimg_MFile *handle); /* 5 */
    int (*tkimg_WritePtr) (tkimg_MFile *handle, const char *src, int count); /* 6 */
    void (*tkimg_ReadBufferPtr) (int onOff); /* 7 */
    void *reserved8;
    void *reserved9;
    int (*tkimg_PhotoPutBlockPtr) (Tcl_Interp *interp, Tk_PhotoHandle handle, Tk_PhotoImageBlock *blockPtr, int x, int y, int width, int height, int flags); /* 10 */
    int (*tkimg_PhotoExpandPtr) (Tcl_Interp *interp, Tk_PhotoHandle handle, int width, int height); /* 11 */
    int (*tkimg_PhotoSetSizePtr) (Tcl_Interp *interp, Tk_PhotoHandle handle, int width, int height); /* 12 */
    void *reserved13;
    void *reserved14;
    void *reserved15;
    void *reserved16;
    void *reserved17;
    void *reserved18;
    void *reserved19;
    void (*tkimg_FixChanMatchProcPtr) (Tcl_Interp **interp, Tcl_Channel *chan, const char **file, Tcl_Obj **format, int **width, int **height); /* 20 */
    void (*tkimg_FixObjMatchProcPtr) (Tcl_Interp **interp, Tcl_Obj **data, Tcl_Obj **format, int **width, int **height); /* 21 */
    void (*tkimg_FixStringWriteProcPtr) (Tcl_DString *data, Tcl_Interp **interp, Tcl_DString **dataPtr, Tcl_Obj **format, Tk_PhotoImageBlock **blockPtr); /* 22 */
    void *reserved23;
    void *reserved24;
    void *reserved25;
    void *reserved26;
    void *reserved27;
    void *reserved28;
    void *reserved29;
    const char * (*tkimg_GetStringFromObjPtr) (Tcl_Obj *objPtr, int *lengthPtr); /* 30 */
    unsigned char * (*tkimg_GetByteArrayFromObjPtr) (Tcl_Obj *objPtr, int *lengthPtr); /* 31 */
    int (*tkimg_ListObjGetElementsPtr) (Tcl_Interp *interp, Tcl_Obj *objPtr, int *argc, Tcl_Obj ***argv); /* 32 */
} TkimgStubs;

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

MODULE_SCOPE const TkimgStubs *tkimgStubsPtr;

#if defined(USE_TKIMG_STUBS) && !defined(USE_TKIMG_STUB_PROCS)

/*
 * Inline function declarations:
 */

#ifndef tkimg_OpenFileChannel
#define tkimg_OpenFileChannel \
	(tkimgStubsPtr->tkimg_OpenFileChannelPtr) /* 0 */
#endif
#ifndef tkimg_ReadInit
#define tkimg_ReadInit \
	(tkimgStubsPtr->tkimg_ReadInitPtr) /* 1 */
#endif
#ifndef tkimg_WriteInit
#define tkimg_WriteInit \
	(tkimgStubsPtr->tkimg_WriteInitPtr) /* 2 */
#endif
#ifndef tkimg_Getc
#define tkimg_Getc \
	(tkimgStubsPtr->tkimg_GetcPtr) /* 3 */
#endif
#ifndef tkimg_Read
#define tkimg_Read \
	(tkimgStubsPtr->tkimg_ReadPtr) /* 4 */
#endif
#ifndef tkimg_Putc
#define tkimg_Putc \
	(tkimgStubsPtr->tkimg_PutcPtr) /* 5 */
#endif
#ifndef tkimg_Write
#define tkimg_Write \
	(tkimgStubsPtr->tkimg_WritePtr) /* 6 */
#endif
#ifndef tkimg_ReadBuffer
#define tkimg_ReadBuffer \
	(tkimgStubsPtr->tkimg_ReadBufferPtr) /* 7 */
#endif
/* Slot 8 is reserved */
/* Slot 9 is reserved */
#ifndef tkimg_PhotoPutBlock
#define tkimg_PhotoPutBlock \
	(tkimgStubsPtr->tkimg_PhotoPutBlockPtr) /* 10 */
#endif
#ifndef tkimg_PhotoExpand
#define tkimg_PhotoExpand \
	(tkimgStubsPtr->tkimg_PhotoExpandPtr) /* 11 */
#endif
#ifndef tkimg_PhotoSetSize
#define tkimg_PhotoSetSize \
	(tkimgStubsPtr->tkimg_PhotoSetSizePtr) /* 12 */
#endif
/* Slot 13 is reserved */
/* Slot 14 is reserved */
/* Slot 15 is reserved */
/* Slot 16 is reserved */
/* Slot 17 is reserved */
/* Slot 18 is reserved */
/* Slot 19 is reserved */
#ifndef tkimg_FixChanMatchProc
#define tkimg_FixChanMatchProc \
	(tkimgStubsPtr->tkimg_FixChanMatchProcPtr) /* 20 */
#endif
#ifndef tkimg_FixObjMatchProc
#define tkimg_FixObjMatchProc \
	(tkimgStubsPtr->tkimg_FixObjMatchProcPtr) /* 21 */
#endif
#ifndef tkimg_FixStringWriteProc
#define tkimg_FixStringWriteProc \
	(tkimgStubsPtr->tkimg_FixStringWriteProcPtr) /* 22 */
#endif
/* Slot 23 is reserved */
/* Slot 24 is reserved */
/* Slot 25 is reserved */
/* Slot 26 is reserved */
/* Slot 27 is reserved */
/* Slot 28 is reserved */
/* Slot 29 is reserved */
#ifndef tkimg_GetStringFromObj
#define tkimg_GetStringFromObj \
	(tkimgStubsPtr->tkimg_GetStringFromObjPtr) /* 30 */
#endif
#ifndef tkimg_GetByteArrayFromObj
#define tkimg_GetByteArrayFromObj \
	(tkimgStubsPtr->tkimg_GetByteArrayFromObjPtr) /* 31 */
#endif
#ifndef tkimg_ListObjGetElements
#define tkimg_ListObjGetElements \
	(tkimgStubsPtr->tkimg_ListObjGetElementsPtr) /* 32 */
#endif

#endif /* defined(USE_TKIMG_STUBS) && !defined(USE_TKIMG_STUB_PROCS) */

/* !END!: Do not edit above this line. */

#endif /* _TKIMGDECLS */

