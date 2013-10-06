/*********************************************
TAB.C
Copyright (C) 1995 by Bruce Shankle
Tabbing function.

Name:           TabProc

Called by ClientWndProc message switch

Created :       01/25/1994      Bruce Shankle

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
#include "tab.h"
#include "help.h"


void TabProc(void)
{
int iLoopCtr;
char tabstring[2];

if(iGlobalUseTabCharacter==0)
        {
        //Send spaces
        for (iLoopCtr=0; iLoopCtr<iGlobalNumTabSpaces; iLoopCtr++)
            {
            WinSendMsg(hwndMLE,MLM_INSERT,MPFROMP(" "),MPVOID);
            }
        }
else
        {
        //Send tab character
        sprintf(tabstring,"%c",9);
        WinSendMsg(hwndMLE,MLM_INSERT,MPFROMP(tabstring),MPVOID);
        }

}

MRESULT EXPENTRY TabSetDlgProc(HWND hwnd, 
                               ULONG msg,
                               MPARAM mp1, 
                               MPARAM mp2)
{
long lNumTabSpaces;
SHORT ix, iy;
SHORT iwidth, idepth;
SWP   swp;
static BOOL bCentered=FALSE;

lNumTabSpaces=iGlobalNumTabSpaces;


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
        case WM_CONTROL:
                //If tabs button is pressed, disable spin button.
                switch(SHORT1FROMMP(mp1))
                {
                case ID_RAD_TABCHAR:
                        {
                        WinEnableWindow(WinWindowFromID(hwnd, ID_SPIN_NUMSPACES),
                                        FALSE);
                        break;
                        }
                case ID_RAD_SPACES:
                        {
                        WinEnableWindow(WinWindowFromID(hwnd, ID_SPIN_NUMSPACES),
                                        TRUE);
                        }
                }
        return 0;

        case WM_INITDLG:

                //Set limits of spin button
                WinSendMsg(
                        WinWindowFromID(hwnd,ID_SPIN_NUMSPACES),
                        SPBM_SETLIMITS,
                        MPFROMLONG(20),
                        MPFROMLONG(1));

                //Initialize spin button value
                WinSendMsg(
                        WinWindowFromID(hwnd,ID_SPIN_NUMSPACES),
                        SPBM_SETCURRENTVALUE,
                        MPFROMLONG(lNumTabSpaces),
                        MPVOID);

                //Initialize radio buttons
                if(iGlobalUseTabCharacter==0)
                        {
                        WinCheckButton(hwnd,ID_RAD_TABCHAR,FALSE);
                        WinCheckButton(hwnd,ID_RAD_SPACES,TRUE);
                        WinEnableWindow(WinWindowFromID(hwnd, ID_SPIN_NUMSPACES),
                                        TRUE);
 
                        }
                else
                        {
                        WinCheckButton(hwnd,ID_RAD_TABCHAR,TRUE);
                        WinCheckButton(hwnd,ID_RAD_SPACES,FALSE);
                        WinEnableWindow(WinWindowFromID(hwnd, ID_SPIN_NUMSPACES),
                                        FALSE);
                        }

                
                WinSetFocus(HWND_DESKTOP,hwnd);
        //Have to return 1 or radio buttons won't be updated
        return MPFROMSHORT(1); 

        case WM_COMMAND:
 
                switch(COMMANDMSG(&msg)->cmd)
                        {
                        case IDB_TABSETOK:
                        
                           //Get spin value for spaces.
                           WinSendMsg(
                                WinWindowFromID(hwnd, ID_SPIN_NUMSPACES),
                                        SPBM_QUERYVALUE,
                                        MPFROMP(&lNumTabSpaces),
                                        MPFROMLONG(0L));

                           //Store number of spaces.
                           iGlobalNumTabSpaces=(int)lNumTabSpaces;

                           //Check to see if it's tabs or spaces.
                           if(WinSendMsg(
                                WinWindowFromID(hwnd, ID_RAD_SPACES),
                                BM_QUERYCHECK,MPVOID,MPVOID))
                                {
                                //Use spaces instead of tabs.
                                iGlobalUseTabCharacter=0;
                                }
                           else
                                {
                                //Use tab characters for tabs.
                                iGlobalUseTabCharacter=1;
                                }
                           bCentered=FALSE;
                           WinDismissDlg(hwnd,TRUE);
                           return 0;

                        case IDB_TABSETCANCEL:
                           bCentered=FALSE;
                           WinDismissDlg(hwnd,TRUE);
                           return 0;
        
                        case IDB_TABHELP:
                           HelpTabs();
                           return 0;

                        }
        break;
        }
return WinDefDlgProc(hwnd, msg, mp1, mp2);
}
//End TabSetDlgProc

