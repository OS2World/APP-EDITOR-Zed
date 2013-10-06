/*********************************************
IMPORT.C
Copyright (C) 1995 by Bruce Shankle
Import file function.

Name:           ImportFileProc
Description:    Handles IDM_IMPORT message from File menu.

Called by ClientWndProc message switch

Created :       03/08/1994      Bruce Shankle

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
#include "import.h"
#include "errormsg.h"
#include "status.h"

int ImportFileProc (void)

{
static FILEDLG fild;                   //File dialog box structure
HWND hwndfild;                 //Handle of file dialog box
HFILE hfile;
ULONG ulAction;
APIRET apiRes;
PCHAR pchar;
IPT lOffset=0;
ULONG cbBytesRead;
PVOID pvBuf;             //pointer to buffer to hold file.
long BUFSIZE=60000;
ULONG cbBytesImported;



////Prepare open common dialog structure.

//Set all fields to 0
memset(&fild,0,sizeof(FILEDLG));

//Set size of structure
fild.cbSize=sizeof(FILEDLG);

//Set dialog type
fild.fl=FDS_CENTER|FDS_OPEN_DIALOG;

//Set title
fild.pszTitle="Import File";

SHOWDIALOG:             //Used by goto when a file is invalid.

//Set initial path and filename
strcpy(fild.szFullFile,szCurrentDirectory);
if(szCurrentDirectory[0]!='*')
  strcat(fild.szFullFile,"*.*");

//Show open common dialog, if a file was not passed in
hwndfild=WinFileDlg(HWND_DESKTOP,hwndClient, &fild);

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
                            WinMessageBox(HWND_DESKTOP, hwndClient, "File not found.",
                                "Open File", 0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
                            break;
                   case ERROR_ACCESS_DENIED:
                            WinMessageBox(HWND_DESKTOP, hwndClient, "Access denied.",
                                "Open File", 0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
                            break;
                   case ERROR_SHARING_VIOLATION:
                           WinMessageBox(HWND_DESKTOP, hwndClient, "Sharing violation.",
                                "Open File", 0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
                           break;
                   default: 
                           WinMessageBox(HWND_DESKTOP, hwndClient, "Can't access file",
                                "Open File", 0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL);


                }
           //Redisplay file open dialog
           goto SHOWDIALOG;
           }
    }
else
    {
    //User pressed cancel
    return 0;
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
////////////////////////////////////////////////////



//Disable file menu
EnableFileMenu(FALSE);

//Open the file
if (DosOpen(fild.szFullFile, &hfile, &ulAction,0,FILE_NORMAL,
                        FILE_OPEN,
                        OPEN_ACCESS_READONLY|OPEN_SHARE_DENYNONE,
                        NULL))
                {
                //Show error message
                ErrorMessage("Could not open file.");
                goto ENDIMPORT;
                }

//Allocate a buffer.
if (DosAllocMem((PPVOID)&pvBuf, BUFSIZE, fALLOC))
{
        //Close the file
        DosClose(hfile);

        //Error allocating memory.
        ErrorMessage("Could not allocate memory to import file.");

        goto ENDIMPORT;
}

//Set MLE import buffer.
WinSendMsg(hwndMLE, 
        MLM_SETIMPORTEXPORT,
        MPFROMP((PBYTE)pvBuf),
        MPFROMLONG(BUFSIZE));

//Disable MLE auto-refresh and hide it.
WinSendMsg(hwndMLE,
        MLM_DISABLEREFRESH,
        MPVOID,
        MPVOID);

//Get offset into MLE
lOffset=LONGFROMMR(WinSendMsg(hwndMLE,
                                        MLM_QUERYSEL,
                                        MPFROMSHORT(MLFQS_CURSORSEL),
                                        MPVOID));

//Read the file and import into MLE window
do
  {
        //Sleep a bit
        DosSleep(100);

        //Read in some of the file
        if (DosRead(hfile, pvBuf, BUFSIZE, &cbBytesRead))
        {
        //Show error message
        ErrorMessage("File read failed.");

        //An error occured
        goto ENDIMPORT;
        }


        //Import text to MLE if there were bytes read in.
        if(cbBytesRead>0)
        {
                cbBytesImported=LONGFROMMR(WinSendMsg(hwndMLE, 
                                                MLM_IMPORT,
                                                MPFROMP(&lOffset),
                                                MPFROMLONG(cbBytesRead)));

                if(!cbBytesImported)
                {
                        //Show error message
                        ErrorMessage("Text not imported.");

                        //Bytes were not imported, error
                        goto ENDIMPORT;

                }
        }


 
} while (cbBytesRead>0); //End do




//Label for end of function, errors goto here.
ENDIMPORT:

//Reset current line and column
ResetLineColumnStatus();

UpdateKeyStatus();

WinSendMsg(hwndMLE, MLM_ENABLEREFRESH, MPVOID, MPVOID);

//Close the file
DosClose(hfile);

//Free the memory
DosFreeMem(pvBuf);

//Enable file menu
EnableFileMenu(TRUE);

return 0;
}
/*End ImportFileProc*/
