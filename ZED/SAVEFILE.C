//////////////////////////////////////////////////////////////////////////////
//SAVEFILE.C
//SaveFile
//
//Saves contents of MLE control to specified filename
//

//Preprocessor directive defines.
#define INCL_WINSTDFILE
#define INCL_WINWINDOWMGR
#define INCL_WINFRAMEMGR
#define INCL_WINMLE
#define INCL_WINSYS
#define INCL_FILEMGR
#define INCL_WINERRORS
#define INCL_DOSFILEMGR

//DosWrite error codes
#define NO_ERROR 0
#define ERROR_ACCESS_DENIED 5
#define ERROR_INVALID_HANDLE  6
#define ERROR_WRITE_PROTECT 19
#define ERROR_NOT_DOS_DISK 26
#define ERROR_WRITE_FAULT 29
#define ERROR_LOCK_VIOLATION 33
#define ERROR_BROKEN_PIPE 109
 



//Include files
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include "zed.h"
#include "status.h"
#include "rcntfile.h"
#include "autosave.h"

int SaveFile(char * szOutfile)
{
ULONG ulBytesWritten;
PVOID pvBuf;
ULONG BUFSIZE=60000;
ULONG EXPORTSIZE;
ULONG ulOffset;
ULONG cbExported;
ULONG ulAction;
HFILE hfile;
APIRET WriteError;
char msgbuffer[80];
int iRes;


//Allocate a buffer for the file
if (DosAllocMem((PPVOID) &pvBuf, BUFSIZE, fALLOC)) return -1; 

//Set file export buffer
WinSendMsg(hwndMLE, MLM_SETIMPORTEXPORT, MPFROMP((PBYTE)pvBuf),
     MPFROMLONG(BUFSIZE));              

//Open the file
if ( DosOpen(szOutfile,
               &hfile,   
               &ulAction,
               0,
               FILE_NORMAL,
               OPEN_ACTION_REPLACE_IF_EXISTS|
               OPEN_ACTION_CREATE_IF_NEW,
               OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE,
               NULL)) 
            {
            //Free the buffer
            DosFreeMem(pvBuf);
            return -2;
            }

//Set starting position for text export
ulOffset = 0;

//Set Write Error
WriteError=0;

//Set bytes written
ulBytesWritten=0;

//Loop through MLE text and export and save it one buffer full at a time.
do {
        EXPORTSIZE=BUFSIZE;
        //Export MLE text
        cbExported=LONGFROMMR(WinSendMsg(hwndMLE, MLM_EXPORT,
                    MPFROMP(&ulOffset), MPFROMP (&EXPORTSIZE) ));

        //Write the file if bytes were exported from MLE.
        if(cbExported)
        WriteError=DosWrite(hfile, pvBuf, cbExported, &ulBytesWritten);


} while ( (cbExported) && ( WriteError==0 ) && (ulBytesWritten!=0) );

//Close the file
DosClose(hfile);

//Free the buffer
DosFreeMem(pvBuf);

//Were bytes exported and not saved?
if ((cbExported>0) && (ulBytesWritten==0))
        {
        WinMessageBox(HWND_DESKTOP,
                        hwndClient,
                        "Insufficient space on targe device.",
                        "Save Error",0,
                        MB_OK|MB_MOVEABLE|MB_ICONEXCLAMATION|MB_SYSTEMMODAL);

        return -3;      //Error
        }

if(WriteError==0)
        {
        //Reset the changed flag
        WinSendMsg(hwndMLE, MLM_SETCHANGED, MPFROMSHORT((BOOL)FALSE), NULL);
        //Changed window title to not have "*"
        sprintf(szTitleBuffer,"%s - [%s]",szAppName,szFileName);
        WinSetWindowText(hwndFrame,szTitleBuffer);

        //Reset title update flag.
        bUpdateTitle=TRUE;

        //Reset MODIFIED on status bar
        bModified=FALSE;
        UpdateKeyStatus();

        //Reset autosave
        iRes=AutoSaveModProc(AUTOSAVEMOD_INITIALIZE);
        iRes=AutoSaveTimerProc(AUTOSAVETIMER_INITIALIZE);
        //Add filename to recent file list
        iRes=AddRecentFile(szFileName);

        return 0;
        }
else
        {
        switch(WriteError)
        {
                case ERROR_ACCESS_DENIED:
                  strcpy(msgbuffer,"Access denied");
                  break;
                case ERROR_INVALID_HANDLE:                   
                  strcpy(msgbuffer,"Invalid handle");
                  break;
               case ERROR_WRITE_PROTECT:
                  strcpy(msgbuffer,"Device is write-protected");
                break;
             case ERROR_NOT_DOS_DISK:
                  strcpy(msgbuffer,"Not a valid disk format");
                break;
             case ERROR_WRITE_FAULT:
                  strcpy(msgbuffer,"Write fault");
                break;
             case ERROR_LOCK_VIOLATION:
                strcpy(msgbuffer,"");
                break;
             case ERROR_BROKEN_PIPE:
                strcpy(msgbuffer,"Broken pipe");
                break;
             default :
                strcpy(msgbuffer,"Error writing file");
                break;
        }
        WinMessageBox(HWND_DESKTOP,hwndClient,msgbuffer,"Save Error",0,
                        MB_OK|MB_MOVEABLE|MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
        return -4;
        }
}
//End SaveFile

