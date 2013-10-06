//////////////////////////////////////////////////////////////////////////////
//GETDIR.C
//GetDirDlgProc
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

MRESULT EXPENTRY GetDirDlgProc (HWND hwnd, 
                                        ULONG msg,
                                        MPARAM mp1, 
                                        MPARAM mp2)
{

SHORT ix, iy;
SHORT iwidth, idepth;
SWP   swp;
static BOOL bCentered=FALSE;
static HWND hwndTextBox;
APIRET apiret;

switch(msg)
    {
    case WM_INITDLG:
        //Get window handle for textbox.
        hwndTextBox=WinWindowFromID(hwnd, TXT_DIR);

        //Set text limit of text box
        WinSendMsg(hwndTextBox, EM_SETTEXTLIMIT, MPFROMSHORT(CCHMAXPATH),
                                        MPVOID);

        //Put default directory in text box.
        WinLoadString(hab, 0, ID_DEFAULTDIR, MAXBUF, (PSZ) szBuffer);
        WinSetWindowText(hwndTextBox, (PSZ) szBuffer);
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
                case IDB_DIR_OK:
                bCentered=FALSE;
                
                    //Get install directory from text box.
                    WinQueryWindowText(hwndTextBox, CCHMAXPATH, (PSZ) szInstallDir);
                    apiret=CheckPath( (PSZ) szInstallDir);

                    //sprintf(szBuffer,"CheckPath returned: %d", apiret);
                    //WinMessageBox(HWND_DESKTOP, hwndClient, szBuffer, "", 0, NULL);
                    
                    if (apiret!=0)
                        {
                        usResponse=MsgBox(ID_QUERYCREATEDIR, ID_APPNAME, MB_YESNO);
                        if (usResponse==MBID_NO) 
                            {
                            //Select all text in text box.
                            WinSendMsg(hwndTextBox, EM_SETSEL, 
                                            MPFROMLONG(0x0FFF0000),
                                            MPVOID);

                            //Set focus back to text box.
                            WinSetFocus(HWND_DESKTOP, hwndTextBox);
                            return 0;
                            }

                        apiret=CreateDirectory( (PSZ) szInstallDir);
                       
                       
                        if (apiret!=0)
                            {
                            //Show error
                            usResponse=MsgBox(ID_CANTCREATEDIR, ID_APPNAME, MB_OK);

                            //Select all text in text box.
                            WinSendMsg(hwndTextBox, EM_SETSEL, 
                                            MPFROMLONG(0x0FFF0000),
                                            MPVOID);

                            //Set focus back to text box.
                            WinSetFocus(HWND_DESKTOP, hwndTextBox);
                            return 0;
                            }


                        }

                    

                    WinDismissDlg(hwnd,TRUE);
                    return 0; //IDB_DIR_OK
            
                case IDB_DIR_CANCEL:
                    bCentered=FALSE;
                    WinDismissDlg(hwnd, FALSE);
                    return 0;
            }
        break;
    }
return WinDefDlgProc(hwnd, msg, mp1, mp2);
}
//End GetDirDlgProc
