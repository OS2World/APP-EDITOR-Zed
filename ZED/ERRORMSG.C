//Preprocessor directive defines.
#define INCL_WINSTDFILE
#define INCL_WINWINDOWMGR
#define INCL_WINFRAMEMGR
#define INCL_WINMLE
#define INCL_WINSYS
#define INCL_FILEMGR
#define INCL_WINERRORS
#define INCL_DOSFILEMGR
#define INCL_DOS
#define INCL_WIN

//Include files
#include <os2.h>

#include "errormsg.h"
#include "zed.h"

////////////////////////////////////////////////////
//ErrorMessage
void ErrorMessage (char * szErrorMsg)
{
WinMessageBox(HWND_DESKTOP,
                hwndClient,
                szErrorMsg,
                szAppName,
                0,
                MB_OK|MB_MOVEABLE|MB_ICONEXCLAMATION|MB_SYSTEMMODAL);

}
//End ErrorMessage
