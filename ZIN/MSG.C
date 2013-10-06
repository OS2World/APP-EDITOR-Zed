///////////////////////////////////////////////////////////
// MSG.C
// Generalized message box routine.
//
// History:     
//    Created:    04/10/95    Bruce Shankle
///////////////////////////////////////////////////////////


#define INCL_WIN
#include <os2.h>
#include "zin.h"

USHORT MsgBox (ULONG ulMessageID, ULONG ulTitleID, ULONG ulExtraStyle)
{
char szMessageText[MAXBUF];
char szMessageTitle[LITTLEBUF];

WinLoadString(hab, 0, ulTitleID, LITTLEBUF, (PSZ) szMessageTitle);
WinLoadString(hab, 0, ulMessageID, MAXBUF, (PSZ) szMessageText);

return WinMessageBox(HWND_DESKTOP, 
                        hwndClient,
                        (PSZ) szMessageText,
                        (PSZ) szMessageTitle,
                        0,
                        MB_MOVEABLE | ulExtraStyle);
}
///////////////////////////////////////////////////////////
