//////////////////////////////////////////////////////////////////////////////
//RM.C
//ReadmeDlgProc
//
//Created :       04/23/1994      Bruce Shankle
//
//

#define INCL_WIN
#include <os2.h>
#include <stdio.h>
#include <string.h>

#include "zin.h"
#include "resource.h"


static HWND hwndMLE;
int LoadReadmeFile (void);


MRESULT EXPENTRY ReadmeDlgProc (HWND hwnd, 
                                        ULONG msg,
                                        MPARAM mp1, 
                                        MPARAM mp2)
{

SHORT ix, iy;
SHORT iwidth, idepth;
SWP   swp;
static BOOL bCentered=FALSE;
int iRes;


switch(msg)
    {
    case WM_INITDLG:

        //Get window handle for MLE
        hwndMLE=WinWindowFromID(hwnd, ID_RM_MLE);

        //Change MLE background to white.
        WinSendMsg(hwndMLE, MLM_SETBACKCOLOR, MPFROMLONG(CLR_WHITE),
                        MPVOID);

        //Change forground color to blue. 
        WinSendMsg(hwndMLE, MLM_SETTEXTCOLOR, MPFROMLONG(CLR_BLUE),
                        MPVOID);

        

        //Load readme file
        iRes=LoadReadmeFile();
        if (iRes!=0)
            {
            WinMessageBox(HWND_DESKTOP, hwndClient,
                "Please be sure to read the README.TXT file.",
                "ZED - Install",
                0,
                MB_MOVEABLE|MB_OK);

                bCentered=FALSE;
                WinDismissDlg(hwnd, TRUE);
                return 0;
            }

        break;

    case WM_PAINT:
        if(!bCentered)
            {
            //Center window
            //Query width and height of Screen device
            iwidth = WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN );
            idepth = WinQuerySysValue( HWND_DESKTOP, SV_CYSCREEN );
            //Query width and height of dialog box
            WinQueryWindowPos( hwnd, (PSWP)&swp );
            //Center dialog box within the Screen
            ix = (SHORT)(( iwidth  - swp.cx ) / 2);
            iy = (SHORT)(( idepth  - swp.cy ) / 2);
            WinSetWindowPos( hwnd, HWND_TOP, ix, iy, 0, 0, SWP_MOVE );            
            bCentered=TRUE;
            }
        break;

    case WM_COMMAND:
        switch(COMMANDMSG(&msg)->cmd)
            {
            case IDB_RM_IUNDERSTAND:
                bCentered=FALSE;
                WinDismissDlg(hwnd,TRUE);
                return 0;
           
            case IDB_RM_CANCELINSTALL:
                bCentered=FALSE;
                WinDismissDlg(hwnd, FALSE);
                return 0;
            
            }


    }
return WinDefDlgProc(hwnd, msg, mp1, mp2);
}



int LoadReadmeFile(void)
{
//Data
CHAR szReadmeFile[CCHMAXPATH+13];
IPT lOffset=0;
HFILE hfile;
ULONG ulAction;
ULONG cbBytesRead;
ULONG cbBytesImported;
PVOID pvBuf;             //pointer to buffer to hold file.
long BUFSIZE=1000;

//Get current directory
GetCurrentDirectory(szReadmeFile);

//Form path+filename
strcat(szReadmeFile,"README.TXT");

//strcpy(szReadmeFile,"README.TXT");


//WinMessageBox(HWND_DESKTOP, hwndClient, szReadmeFile,"", NULL, 0);

//Clear the MLE window.
WinSetWindowText(hwndMLE,"");

//Set text limit of MLE control to unlimited
WinSendMsg(hwndMLE, MLM_SETTEXTLIMIT, MPFROMLONG(-1), MPVOID);

//Open the file.
if (DosOpen(szReadmeFile, &hfile, &ulAction,0,FILE_NORMAL,
             FILE_OPEN,
             OPEN_ACCESS_READONLY|OPEN_SHARE_DENYNONE,
             NULL)) goto ERROR_READFILE;

//Allocate a buffer.
if (DosAllocMem((PPVOID)&pvBuf, BUFSIZE, fALLOC)) goto ERROR_READFILE;

//Set MLE import buffer.
WinSendMsg(hwndMLE, 
        MLM_SETIMPORTEXPORT,
        MPFROMP((PBYTE)pvBuf),
        MPFROMLONG(BUFSIZE));

//Set insertion point
lOffset = 0;

//Read the file and import into MLE window
do
    {
    //Read in some of the file
    if (DosRead(hfile, pvBuf, BUFSIZE, &cbBytesRead)) goto ERROR_READFILE;

    //Import text to MLE if there were bytes read in.
    if(cbBytesRead>0)
        {
        cbBytesImported=LONGFROMMR(WinSendMsg(hwndMLE, 
                                                MLM_IMPORT,
                                                MPFROMP(&lOffset),
                                                MPFROMLONG(cbBytesRead)));

        if(!cbBytesImported) goto ERROR_READFILE;
        }

} while (cbBytesRead>0); //End do

//Close the file
DosClose(hfile);

//Free the memory
DosFreeMem(pvBuf);

//Success
return 0;

//Handle errors
ERROR_READFILE:
    //Close the file
    DosClose(hfile);

    //Free the memory
    DosFreeMem(pvBuf);

    return -1;
}