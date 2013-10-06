//////////////////////////////////////////////////////////////////////////////
//STATUS.C
//StatusDlgProc
//
//Created :       01/08/1994      Bruce Shankle
//
//Processes the about dialog messages.
//

#define INCL_WIN
#include <os2.h>
#include <stdio.h>
#include "zin.h"
#include "resource.h"


static HWND hwndStatusTextBox;
static HWND hwndStatus;


MRESULT EXPENTRY StatusDlgProc (HWND hwnd, 
                                        ULONG msg,
                                        MPARAM mp1, 
                                        MPARAM mp2)
{

SHORT ix, iy;
SHORT iwidth, idepth;
SWP   swp;
static BOOL bCentered=FALSE;


switch(msg)
    {
    case WM_INITDLG:

        //Get window handle for textbox.
        hwndStatusTextBox=WinWindowFromID(hwnd, ID_TXT_STATUS);

        //Set text limit of text box
        WinSendMsg(hwndStatusTextBox, EM_SETTEXTLIMIT, MPFROMSHORT(CCHMAXPATH),
                                        MPVOID);

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

    }
return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

void LoadStatusDlg(void)
{
//Load the status dialog window.
hwndStatus=WinLoadDlg(HWND_DESKTOP, hwndClient, StatusDlgProc, 0L, IDD_STATUS, NULL);
}

void DestroyStatusDlg(void)
{
WinDismissDlg(hwndStatus, FALSE);
}

void SetStatus (ULONG ulMessageID)
{
CHAR szStatusText[MAXBUF];
WinLoadString(hab, 0, ulMessageID, MAXBUF, (PSZ) szStatusText);
WinSetWindowText(hwndStatusTextBox, szStatusText);

}