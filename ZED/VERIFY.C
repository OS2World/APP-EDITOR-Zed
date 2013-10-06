//////////////////////////////////////////////////////////////////////////////
//VERIFY.C
//VerifyDlgProc
//
//Procedure for replace/verify dialog box.
//Created:      01/23/1994      Bruce Shankle
//
#define INCL_WINSTDFILE
#define INCL_WINWINDOWMGR
#define INCL_WINFRAMEMGR
#define INCL_WINMLE
#define INCL_WINSYS
#define INCL_FILEMGR
#define INCL_WINERRORS
#define INCL_DOSFILEMGR
#define INCL_DOS
#define INCL_WIN

#include <os2.h>
#include <string.h>
#include "zed.h"
#include "verify.h"

SHORT ix, iy;
SHORT iwidth, idepth;
SWP   swp;
static BOOL bCentered=FALSE;


MRESULT EXPENTRY VerifyDlgProc (HWND hwnd,
                                        ULONG msg,
                                        MPARAM mp1, 
                                        MPARAM mp2)
{

//ULONG ulGlobalCaseSensitive;
MLE_SEARCHDATA mlesearch;
MRESULT mres;
IPT iptCursorPosition;

mlesearch.cb=sizeof(mlesearch);
mlesearch.pchFind=szGlobalSearchString;
mlesearch.pchReplace=szGlobalReplaceString;
mlesearch.iptStart=0;
mlesearch.iptStop=-1;
mlesearch.cchFind=strlen(szGlobalSearchString);
mlesearch.cchReplace=strlen(szGlobalReplaceString);



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

        case WM_INITDLG:
        //Find and select first instance.
        mres=WinSendMsg
                       (hwndMLE,
                        MLM_SEARCH,
                        MPFROMLONG(MLFSEARCH_SELECTMATCH|ulGlobalCaseSensitive),
                        MPFROMP(&mlesearch ));
        //If text was not initially found, display a message and unload
        //verify dialog box.
        if(!mres)
                {
                WinMessageBox(HWND_DESKTOP,hwnd,"Text not found","Verify",0,
                                MB_OK|MB_MOVEABLE|MB_SYSTEMMODAL);
                WinDismissDlg(hwnd,TRUE);
                }
 
        return 0;

        case WM_COMMAND:
                switch(COMMANDMSG(&msg)->cmd)
                        {
                        case IDB_VERIFYCANCEL:
                           bCentered=FALSE;
                           WinDismissDlg(hwnd,TRUE);
                           return 0;//IDB_VERIFYCANCEL

                        case IDB_VERIFYYES:
                        //Insert text and find next.
                        WinSendMsg
                                (hwndMLE,
                                MLM_INSERT,
                                MPFROMP(szGlobalReplaceString),
                                MPVOID);

                           //return 0;//IDB_VERIFYYES

                        case IDB_VERIFYNO:

                          //Get current cursor position
                          iptCursorPosition=LONGFROMMR(WinSendMsg
                                (hwndMLE,
                                MLM_QUERYSEL,
                                MPFROMSHORT(MLFQS_CURSORSEL),
                                MPVOID));

                          //Change search structure to new insertion point.
                          mlesearch.iptStart=iptCursorPosition;
 
                          //Find and select next instance.
                                mres=WinSendMsg
                               (hwndMLE,
                                MLM_SEARCH,
                                MPFROMLONG(MLFSEARCH_SELECTMATCH|ulGlobalCaseSensitive),
                                MPFROMP(&mlesearch ));
                          //If text was not found, display a message and unload
                          //verify dialog box.
                          if(!mres)
                            {
                             WinMessageBox(HWND_DESKTOP,hwnd,
                                "Finished","Verify",0,
                                MB_OK|MB_MOVEABLE|MB_SYSTEMMODAL);
                             bCentered=FALSE;
                             WinDismissDlg(hwnd,TRUE);
                            }
                        return 0;//IDB_VERIFYNO

                        }
        break;
        }
return WinDefDlgProc(hwnd, msg, mp1, mp2);

}
