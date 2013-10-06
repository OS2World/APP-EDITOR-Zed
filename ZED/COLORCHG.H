////////////////////////////////////
//COLORCHG.H
//
//Header file for COLORCHG.C
//
//Created:    02/21/1995    Bruce Shankle

#ifdef COLORCHG_MAIN
#define COLORCHG_GLOBAL
#else
#define COLORCHG_GLOBAL extern
#endif

#define IDD_COLORCHANGE             858
#define IDB_COLORCHANGETEXT         859
#define IDB_COLORCHANGEBACKGROUND   860
#define IDB_COLORCHANGEHELP         861
#define IDB_COLORCHANGECANCEL       862

#define COLORCHANGE_BACKGROUND   1
#define COLORCHANGE_TEXT            2
#define COLORCHANGE_CANCEL         3

//Variables
COLORCHG_GLOBAL glColorChgAction;

//Function decls.
MRESULT EXPENTRY ColorChgDlgProc (HWND hwnd, 
                                        ULONG msg,
                                        MPARAM mp1, 
                                        MPARAM mp2);

