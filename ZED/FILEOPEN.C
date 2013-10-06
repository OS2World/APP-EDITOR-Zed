/*********************************************
FILEOPEN.C
Copyright (C) 1995 by Bruce Shankle
File open function.

Name:           FileOpenProc
Description:    Handles IDM_OPEN message from File menu.

Called by ClientWndProc message switch

Created :       01/06/1994      Bruce Shankle

Parameters       :       HWND hwnd       Parent window handle.
*/

/*Preprocessor directive defines.*/
#define INCL_WINSTDFILE
#define INCL_WINWINDOWMGR
#define INCL_WINFRAMEMGR
#define INCL_WINMLE
#define INCL_WINSYS
#define INCL_FILEMGR
#define INCL_WINERRORS
#define INCL_WIN
#define INCL_DOSFILEMGR
#define INCL_DOS
#define INCL_DOSPROCESS

//Include files
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <process.h>
#include "zed.h"
#include "rcntfile.h"

int FileOpenProc (HWND hwnd, char * szOpenFileName)
{
static FILEDLG fild;                   //File dialog box structure
HWND hwndfild;                 //Handle of file dialog box
HFILE hfile;
ULONG ulAction;
APIRET apiRes;
BOOL bTextChanged;
USHORT mbresponse;
PCHAR pchar;
BOOL bUsePassedFilename;

//Was a filename passed in?
if (szOpenFileName[0]!=0) bUsePassedFilename=TRUE;
else bUsePassedFilename=FALSE;

//Query MLE control to see if text has changed.
bTextChanged=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_QUERYCHANGED,
                        MPVOID,
                        MPVOID));

//If text has changed see if user wants to save it.
if(bTextChanged)
        {
        sprintf(buffer,"The text in the file has changed.\n\n");
        strcat(buffer,"Do you want to save the changes?");
        mbresponse=WinMessageBox(HWND_DESKTOP,
                                 hwndClient,
                                 buffer,
                                 szAppName,
                                 0,
                                 MB_YESNOCANCEL|
                                 MB_ICONQUESTION|
                                 MB_MOVEABLE|MB_SYSTEMMODAL);
        if(mbresponse==MBID_YES)
           {
           if(szFileName[0]==0) FileSaveAsProc(hwndClient);
           else FileSaveProc(hwndClient);
           //If cancel was hit on FileSave dialog, then filename will still be null.
           if(szFileName[0]==0) return 0;
           }
        if(mbresponse==MBID_CANCEL)
           {
           return 0;
           }
        }

////Prepare open common dialog structure.

//Set all fields to 0
memset(&fild,0,sizeof(FILEDLG));

//Set size of structure
fild.cbSize=sizeof(FILEDLG);

//Set dialog type
fild.fl=FDS_CENTER|FDS_OPEN_DIALOG;//|FDS_PRELOAD_VOLINFO;

//Set title
fild.pszTitle="Open";

SHOWDIALOG:             //Used by goto when a file is invalid.

//Set initial path and filename
strcpy(fild.szFullFile,szCurrentDirectory);
if(szCurrentDirectory[0]!='*')
  strcat(fild.szFullFile,"*.*");


//Show open common dialog, if a file was not passed in
if (!bUsePassedFilename)
        {
        hwndfild=WinFileDlg(HWND_DESKTOP,hwnd, &fild);
        }
else    //A file name was passed in, so use that.
        {
        strcpy(fild.szFullFile, szOpenFileName);
        fild.lReturn=DID_OK;
        bUsePassedFilename=FALSE;
        }

//If OK was pressed on dialog, open selected file.
if (fild.lReturn==DID_OK)
        {
        //Convert to upper case if not in case sensitive mode
        if(!gbCaseSensitiveFileNames) strupr(fild.szFullFile);

        //Try to open the file to make sure it is valid.
         apiRes=DosOpen(fild.szFullFile,
               &hfile,   
               &ulAction,
               0,
               FILE_NORMAL,
               FILE_OPEN,
               OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE,
               NULL);

        //Close the file
        DosClose(hfile);
        if (apiRes)
           {
           //Handle bad return code from DosOpen.
           switch(apiRes)
                {
                   case ERROR_FILE_NOT_FOUND:
                   case ERROR_PATH_NOT_FOUND:
                            WinMessageBox(HWND_DESKTOP, hwnd, "File not found.",
                                "Open File", 0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
                            break;
                   case ERROR_ACCESS_DENIED:
                            WinMessageBox(HWND_DESKTOP, hwnd, "Access denied.",
                                "Open File", 0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
                            break;
                   case ERROR_SHARING_VIOLATION:
                           WinMessageBox(HWND_DESKTOP, hwnd, "Sharing violation.",
                                "Open File", 0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
                           break;
                   default: 
                           WinMessageBox(HWND_DESKTOP, hwnd, "Can't access file",
                                "Open File", 0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);


                }
           //Redisplay file open dialog
           goto SHOWDIALOG;
           }

/////////////////////////////////////////////////////////
//Save the current directory.
//This is so the next time the dialog is brought up
//it is in the expected directory.
//Get the file name from the dialog box structure
strcpy(szCurrentDirectory, fild.szFullFile);
//Set a pointer to end of filename.
pchar=szCurrentDirectory+sizeof(szCurrentDirectory);
//Back up until we hit a "\"
while((*pchar!='\\') && (pchar!=szCurrentDirectory)) pchar--;
//Move forward one and set a null
if(*pchar=='\\')
        {
        pchar++;
        *pchar='\0';
        }
else
        {
        //Else this is not a path, so delete it
        strcpy(szCurrentDirectory,"");
        }

//WinMessageBox(HWND_DESKTOP,hwndClient,szCurrentDirectory,
//                    "Current Directory",0,NULL);

////////////////////////////////////////////////////



        //Disable file menu
        EnableFileMenu(FALSE);

        //Set continue flag for read thread communication.
        iContinueOpen=1;
        iReadFileFinished=0;


        //Start thread to read in the file.
        ReadThreadID=_beginthread (ReadFile,NULL,STACKSIZE,fild.szFullFile);
        if (ReadThreadID==-1) WinMessageBox(HWND_DESKTOP,hwnd,"Could not start thread.",
                                szAppName,0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);

        //Set the priority of the file open thread to low
        else
        {
        
        apiRes=DosSetPriority(PRTYS_THREAD,PRTYC_REGULAR,
                                PRTYD_MINIMUM,ReadThreadID);
        }

        }//End if 
return 0;
}
/*End FileOpenProc*/


                                                 
