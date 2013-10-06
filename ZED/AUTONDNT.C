////////////////////////////////////////////
//AUTONDNT.C
//Created:	02/12/1995	Bruce Shankle
//This file contains routines for auto-indent feature

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
#define INCL_WINCLIPBOARD
#define INCL_GPI
#define AUTONDNT_MAIN


//Include files.
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <process.h>
#include "zed.h"
#include "autondnt.h"

int AutoIndentProc(void)
{
ULONG lLineNumber;
IPT iptFirstChar;
ULONG lLineLength;
PCHAR pvBuf;
ULONG ulOffset;
ULONG cbExported;
ULONG ulExportSize;
LONG lLoopCtr;


//Make MLE read only
//WinSendMsg(hwndMLE,MLM_SETREADONLY,MPFROMSHORT(TRUE),MPVOID);

//Disable refresh of MLE
//WinSendMsg(hwndMLE,MLM_DISABLEREFRESH,MPVOID,MPVOID);

//Get line number
lLineNumber=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_LINEFROMCHAR,
                        MPFROMLONG(-1L),
                        MPVOID));
//We want previous line.
lLineNumber--;

//Get first character
iptFirstChar=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_CHARFROMLINE,
                        MPFROMLONG(lLineNumber),
                        MPVOID));

//Store first char to ulOffset
ulOffset=iptFirstChar;

//Get length of line
lLineLength=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_QUERYLINELENGTH,
                        MPFROMLONG(iptFirstChar),
                        MPVOID));

//Store length in ExportSize
ulExportSize=lLineLength;

//Allocate memory to store line.
if (DosAllocMem((PPVOID) &pvBuf, lLineLength, fALLOC))
    {
    WinMessageBox(HWND_DESKTOP,hwndClient,"Could not allocate memory for autoindent.",
                    szAppName,0,MB_OK|MB_MOVEABLE|MB_ICONEXCLAMATION);
    //Renable refresh of MLE
    WinSendMsg(hwndMLE,MLM_ENABLEREFRESH,MPVOID,MPVOID);


    return -1;
    }

//Set MLE export buffer
WinSendMsg(hwndMLE, MLM_SETIMPORTEXPORT, MPFROMP((PBYTE)pvBuf),
     MPFROMLONG(lLineLength));

//Export MLE text
cbExported=LONGFROMMR(WinSendMsg(hwndMLE, MLM_EXPORT,
    MPFROMP(&ulOffset), MPFROMP (&ulExportSize) ));

//Find end of spaces or tabs
for(lLoopCtr=0; lLoopCtr<lLineLength-1; lLoopCtr++)
    {
    if ( (*(pvBuf+lLoopCtr)!=32) &&
         (*(pvBuf+lLoopCtr)!=9)
       )
       {
       //Terminate the string
       *(pvBuf+lLoopCtr)=0;
       //Insert into MLE
       WinSendMsg(hwndMLE,MLM_INSERT,MPFROMP(pvBuf),MPVOID);
       break;
       }

    }

//Free the memory.
DosFreeMem(pvBuf);

//Renable refresh of MLE
//WinSendMsg(hwndMLE,MLM_ENABLEREFRESH,MPVOID,MPVOID);

//Make MLE non - read only
//WinSendMsg(hwndMLE,MLM_SETREADONLY,MPFROMSHORT(FALSE),MPVOID);

return 0;

}
