///////////////////////////////////////////////////////////
// ZIN.C
// Main file for ZIN installation program.
//
// History:
//    Created:    04/09/95    1.0    Bruce Shankle
///////////////////////////////////////////////////////////

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <string.h>
#define MAIN
#include "zin.h"
#include "resource.h"
#include "olf.h"
#include "chkfiles.h"

int main (void)
{

SHORT ix, iy;
SHORT iwidth, idepth;
SWP   swp;
ULONG ulResult;
int iRes;

static CHAR szClientClass[]="ZIN";
static ULONG flFrameFlags= FCF_TITLEBAR | FCF_SYSMENU |
                               FCF_TASKLIST | FCF_ICON | FCF_SHELLPOSITION|
                                FCF_BORDER | FCF_NOBYTEALIGN;

   
hab=WinInitialize(0);
hmq=WinCreateMsgQueue(hab, 0);

//Register processing class
WinRegisterClass(
            hab,
            szClientClass,
            ClientWndProc,
            CS_SIZEREDRAW,
            0);

//Create Window
hwndFrame=WinCreateStdWindow(
            HWND_DESKTOP,
            WS_VISIBLE,
            &flFrameFlags,
            szClientClass,
            NULL,
            0L,
            0,        //Module handle for resources
            ID_RESOURCE,        //ID of resources
            &hwndClient);


//Size window
WinSetWindowPos( hwndFrame, HWND_TOP, 0, 0, 640, 480, SWP_SIZE );            

//Center window
iwidth = WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN );
idepth = WinQuerySysValue( HWND_DESKTOP, SV_CYSCREEN );
WinQueryWindowPos( hwndFrame, (PSWP)&swp );
ix = (SHORT)(( iwidth  - swp.cx ) / 2);
iy = (SHORT)(( idepth  - swp.cy ) / 2);
WinSetWindowPos( hwndFrame, HWND_TOP, ix, iy, 0, 0, SWP_MOVE );            

//Set focus to edit window.
WinSetFocus(HWND_DESKTOP,hwndClient);

//Load application title
WinLoadString(hab, 0, ID_APPNAME, LITTLEBUF, (PSZ) szAppName);

//Set title of frame.
WinSetWindowText(hwndFrame, (PSZ) szAppName);
 
  
//Show the window
WinShowWindow(hwndFrame,TRUE);

//Get source directory.
GetCurrentDirectory(szSourceDir);


//Load first message
FIRSTMSG:
usResponse=MsgBox(ID_FIRSTMSG, ID_APPNAME, MB_OKCANCEL);
WinUpdateWindow(hwndClient);

if (usResponse==MBID_OK)
    {

    //Show readme.txt file
    ulResult=WinDlgBox(HWND_DESKTOP, hwndClient, ReadmeDlgProc,
                NULLHANDLE, IDD_README, NULL);

    if (ulResult==FALSE) 
        {
        //The installation has been cancelled.
        usResponse=MsgBox(ID_CANCELINSTALL, ID_APPNAME, MB_OK);    
        goto END_INSTALL;
        }


    ulResult=WinDlgBox(HWND_DESKTOP, hwndClient, GetDirDlgProc,
                NULLHANDLE, IDD_GETDIR, NULL);
    
    if (ulResult==FALSE) goto FIRSTMSG;

    //Update the window
    WinUpdateWindow(hwndClient);

    //Load status dialog
    LoadStatusDlg();

    //Make sure all files exist
    iRes=CheckFiles(szSourceDir);
    if (iRes!=0)
        {
        DestroyStatusDlg();
        usResponse=MsgBox(ID_FILENOTFOUND, ID_APPNAME, MB_OK);
        goto END_INSTALL;
        }

    //Copy the files
    iRes=CopyFiles(szSourceDir, szInstallDir);
    if (iRes!=0)
        {
        DestroyStatusDlg();
        usResponse=MsgBox(ID_FILESNOTCOPIED, ID_APPNAME, MB_OK);
        goto END_INSTALL;
        }

    iRes=PatchZed(szInstallDir);
    if (iRes!=0)
        {
        DestroyStatusDlg();
        usResponse=MsgBox(ID_PATCHFAILED, ID_APPNAME, MB_OK);
        goto END_INSTALL;
        }

    
    iRes=CreateObject(szInstallDir);
    if (iRes!=0) usResponse=MsgBox(ID_CANTCREATEOBJECT, ID_APPNAME, MB_OK);

    //unload status window
    DestroyStatusDlg();

    //Update the window
    WinUpdateWindow(hwndClient);

    //Success
    usResponse=MsgBox(ID_SUCCESS, ID_APPNAME, MB_OK);
    goto END_INSTALL;
    
    //Main message loop
    while (WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0))
        WinDispatchMsg(hab, &qmsg);

    }


if (usResponse==MBID_CANCEL)
    {
    //The installation has been cancelled.
    usResponse=MsgBox(ID_CANCELINSTALL, ID_APPNAME, MB_OK);    
    }

END_INSTALL:
//Cleanup
WinDestroyWindow(hwndFrame);
WinDestroyMsgQueue(hmq);
WinTerminate(hab);

//End
return 0;
}
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
//ClientWndProc

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, 
                                        MPARAM mp1, MPARAM mp2)

{
HPS hps;
RECTL rcl;

switch (msg)
    {
    case WM_CREATE:
        return 0;

    case WM_PAINT:
        hps=WinBeginPaint(hwnd, NULLHANDLE, NULL);
        WinQueryWindowRect(hwnd, &rcl);
        GpiErase(hps);
        WinFillRect(hps, &rcl, CLR_BLUE);
        PaintClient(hps, 100, 100);

        WinEndPaint(hps);
        return 0;

    case WM_DESTROY:
        return 0;
    }
return WinDefWindowProc(hwnd, msg, mp1, mp2);
}
///////////////////////////////////////////////////////////

