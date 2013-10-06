//////////////////////////////////////////////////////////////////////////////
//FILESVAS.C
//FileSaveAsProc
//Handles IDM_SAVEAS message from File menu.
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
#include "rcntfile.h"

void FileSaveAsProc (HWND hwnd)
{
//File dialog box structure and handle
FILEDLG fild;                   
HWND hwndfild;
int iRes;
HFILE hfile;
ULONG ulAction;
SHORT mbresponse;
PCHAR pchar;




////Prepare open common dialog structure.

//Set all fields to 0
memset(&fild,0,sizeof(FILEDLG));

//Set size of structure
fild.cbSize=sizeof(FILEDLG);

//Set dialog type
fild.fl=FDS_CENTER|FDS_OPEN_DIALOG;

//Set title
fild.pszTitle="Save As";

//Set current directory, (This is overridden if szFileName is not null)
if ((szCurrentDirectory[0]!=0)&&(szCurrentDirectory[0]!='*'))
strcpy(fild.szFullFile,szCurrentDirectory);
else strcpy(fild.szFullFile,"");

//Set initial filename
if (szFileName[0]==0) 
   strcat(fild.szFullFile,"UNTITLED.TXT");
else strcpy(fild.szFullFile,szFileName);

//Show open common dialog.
SHOWDIALOG:                            
hwndfild=WinFileDlg(HWND_DESKTOP,hwndClient, &fild);

//If OK was pressed on dialog, open selected file.
if (hwndfild &&( fild.lReturn==DID_OK)) 
        {
        //Convert to upper case if not in case sensitive mode.
        if (!gbCaseSensitiveFileNames) strupr(fild.szFullFile);

        //Open the file
        DosOpen(fild.szFullFile,
               &hfile,   
               &ulAction,
               0,
               FILE_NORMAL,
               FILE_OPEN,
               OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE,
               NULL);

        //Close the file
        DosClose(hfile);

        //Does the file already exist. If so, verify user wants to replace it.
        if (ulAction==FILE_EXISTED)
           {
           mbresponse=WinMessageBox(HWND_DESKTOP, hwnd, "File already exists. Replace?",
                                        szAppName, 0,
                                        MB_QUERY|
                                        MB_YESNOCANCEL|
                                        MB_MOVEABLE|MB_SYSTEMMODAL);

           if (mbresponse==MBID_NO)
                {                  
                goto SHOWDIALOG;
                }

           if (mbresponse==MBID_CANCEL)
                {
                return;
                }
           
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
////////////////////////////////////////////////////

        //Save the file.
        iRes=SaveFile(fild.szFullFile);
        if(iRes==0)
                {
                //Change window title to new file name.
                sprintf(szTitleBuffer,"%s - [%s]",szAppName,fild.szFullFile);
                WinSetWindowText(hwndFrame,szTitleBuffer);
 
                //Copy the name of the file to szFileName
                strcpy(szFileName,fild.szFullFile);
                }
      else
                {
                //File could not be save.
                WinMessageBox(HWND_DESKTOP, hwnd,
                                "File file was not saved.",
                                "Warning", 0,
                                MB_OK|
                                MB_MOVEABLE|
                                MB_ICONEXCLAMATION|MB_SYSTEMMODAL
                                );

                } //End if-else

        }//End if
}
//End FileSaveAsProc

