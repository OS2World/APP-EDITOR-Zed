//////////////////////////////////////////////////////////////////////////////
//FileSaveProc
//
//Saves file when it already has a name.

//Preprocessor directive defines.
#define INCL_WINSTDFILE
#define INCL_WINWINDOWMGR
#define INCL_WINFRAMEMGR
#define INCL_WINMLE
#define INCL_WINSYS
#define INCL_FILEMGR
#define INCL_WINERRORS
#define INCL_DOSFILEMGR

//Include files
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include "zed.h"

void FileSaveProc (HWND hwnd)
{

int iRes;
iRes=SaveFile(szFileName);
if (iRes!=0)
   {
   WinMessageBox(HWND_DESKTOP, hwnd, 
                "File was not saved.",
                "Warning", 0,
                MB_OK|
                MB_MOVEABLE|
                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
   }
}
//

