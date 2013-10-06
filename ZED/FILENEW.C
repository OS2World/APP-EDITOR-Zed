//////////////////////////////////////////////////////////////////////////////
//FILENEW.C
//Copyright (C) 1995 by Bruce Shankle
//File new function.
//
//FileNewProc
//Handles IDM_NEW message from File menu.
//
//Called by ClientWndProc message switch
//
//
//Created :       01/06/1994      Bruce Shankle
//
//Parameters       :       HWND hwnd       Parent window handle.
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

//Include files
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include "zed.h"
#include "status.h"
#include "rcntfile.h"

void FileNewProc (HWND hwnd)
{

USHORT mbresponse;
BOOL bTextChanged;
int iRes;

//Query MLE control to see if text has changed.
bTextChanged=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_QUERYCHANGED,
                        MPVOID,
                        MPVOID));

//If text has changed, see if user wants to save it.
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
           if(szFileName[0]!=0)
              {
                //Change title to "Clearing text-edit memory..."
                sprintf(szTitleBuffer,"%s - Clearing memory...",szAppName);
                WinSetWindowText(hwndFrame,szTitleBuffer);

                 //Clear the text window
                WinSetWindowText(hwndMLE,"");

                //Set the window frame title to appname + untitled
                sprintf(szTitleBuffer,"%s - [untitled]",szAppName);
                WinSetWindowText(hwndFrame,szTitleBuffer);

                //Reset changed flag on MLE
                WinSendMsg(hwndMLE, MLM_SETCHANGED, MPFROMSHORT((BOOL)FALSE), NULL);

                //Set filename to nothing
                strcpy(szFileName,"");

                //Reset current line and column
                ResetLineColumnStatus();

                //Reset MODIFIED status
                bModified=FALSE;
                UpdateKeyStatus();
 

              }
           }
        if(mbresponse==MBID_NO)
           {
                //Change title to "Clearing text-edit memory..."
                sprintf(szTitleBuffer,"%s - Clearing memory...",szAppName);
                WinSetWindowText(hwndFrame,szTitleBuffer);
 
                 //Clear the text window
                WinSetWindowText(hwndMLE,"");

                //Set the window frame title to appname + untitled
                sprintf(szTitleBuffer,"%s - [untitled]",szAppName);
                WinSetWindowText(hwndFrame,szTitleBuffer);

                //Reset changed flag on MLE
                WinSendMsg(hwndMLE, MLM_SETCHANGED, MPFROMSHORT((BOOL)FALSE), NULL);

                //Add the filename to recent file list.
                iRes=AddRecentFile(szFileName);


                //Set filename to nothing
                strcpy(szFileName,"");

                //Reset current line and column
                ResetLineColumnStatus();

                //Reset MODIFIED status
                bModified=FALSE;
                UpdateKeyStatus();

                //Reset update title flag.
                bUpdateTitle=TRUE;


           }
        
        }
else    {
        //Change title to "Clearing text-edit memory..."
        sprintf(szTitleBuffer,"%s - Clearing memory...",szAppName);
        WinSetWindowText(hwndFrame,szTitleBuffer);

        //Add the filename to recent file list.
        iRes=AddRecentFile(szFileName);

        //Clear the text window
        WinSetWindowText(hwndMLE,"");

        //Set the window frame title to appname + untitled
        sprintf(szTitleBuffer,"%s - [untitled]",szAppName);
        WinSetWindowText(hwndFrame,szTitleBuffer);

        //Reset changed flag on MLE
        WinSendMsg(hwndMLE, MLM_SETCHANGED, MPFROMSHORT((BOOL)FALSE), NULL);

        //Set filename to nothing
        strcpy(szFileName,"");
 
        //Reset current line and column
        ResetLineColumnStatus();

        //Reset MODIFIED status
        bModified=FALSE;
        UpdateKeyStatus();
        
        //Reset update title flag.
        bUpdateTitle=TRUE;
        
    

        }

hwnd=hwnd;
}
//End FileNewProc
