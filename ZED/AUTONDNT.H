////////////////////////////////////////////////////
//AUTONDNT.H
//Header file for AUTONDNT.C
//Created:	02/04/1995	Bruce Shankle

#ifdef AUTONDNT_MAIN
#define AUTONDNT_GLOBAL
#else
#define AUTONDNT_GLOBAL extern
#endif

AUTONDNT_GLOBAL BOOL bUseAutoIndent; //Flag for whether or not to use autoindent.
int AutoIndentProc(void);


