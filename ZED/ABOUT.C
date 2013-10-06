//////////////////////////////////////////////////////////////////////////////
//ABOUT.C
//AboutDlgProc
//
//Created :       01/08/1994      Bruce Shankle
//
//Processes the about dialog messages.
//

#define INCL_WIN
#include <os2.h>
#include "zed.h"
#include "about2.h"

MRESULT EXPENTRY AboutDlgProc (HWND hwnd, 
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
                        case IDB_ABOUT_OK:
                           bCentered=FALSE;
                           WinDismissDlg(hwnd,TRUE);
                           return 0;
                        }
        break;
        }
return WinDefDlgProc(hwnd, msg, mp1, mp2);
}
//End AboutDlgProc
