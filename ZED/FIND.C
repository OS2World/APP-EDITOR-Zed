//////////////////////////////////////////////////////////////////////////////
//FIND.C
//FindDlgProc
//
//Procedure for Find dialog box.
//Created:      01/15/1994      Bruce Shankle
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
#include "find.h"


MRESULT EXPENTRY FindDlgProc (HWND hwnd,
                                        ULONG msg,
                                        MPARAM mp1, 
                                        MPARAM mp2)
{

//Structure for searching MLE window.
static MLE_SEARCHDATA mlesearch;

//Buffer to hold search string
static CHAR szSearchString[64];
static CHAR szReplaceString[64];
LONG lRetLen;
MRESULT mres;
ULONG ulCaseSensitive;
SHORT ix, iy;
SHORT iwidth, idepth;
SWP   swp;
static BOOL bCentered=FALSE;


mlesearch.cb=sizeof(mlesearch);
mlesearch.pchFind=szSearchString;
mlesearch.pchReplace=szReplaceString;
mlesearch.iptStart=-1;
mlesearch.iptStop=-1;



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
                //When the find dialog is created, if user has selected
                //text in the main window, copy it into the find text box.
                
                WinSendMsg(hwndMLE,
                        MLM_QUERYSELTEXT,
                        MPFROMP(&szSearchString),
                        MPFROMSHORT(sizeof(szSearchString)));

                //Set the case sensitive button.
                if(ulGlobalCaseSensitive)
                WinCheckButton(hwnd,ID_CASESENSITIVE,TRUE);
                else
                WinCheckButton(hwnd,ID_CASESENSITIVE,FALSE);


                //If no text was selected, get text from global search string.
                if(szSearchString[0]==0)
                   strcpy(szSearchString,szGlobalSearchString);

                //Put the selected text into the dialog box entry field.
                WinSetWindowText(WinWindowFromID(hwnd, ID_FINDTEXT), szSearchString);

                WinSetFocus(HWND_DESKTOP,WinWindowFromID(hwnd,ID_FINDTEXT));

                //Make the text in find window selected.
                WinSendMsg(WinWindowFromID(hwnd,ID_FINDTEXT), EM_SETSEL,
                                        MPFROMLONG(0x0FFF0000),MPVOID);

                                                
        return 0;
        case WM_COMMAND:
                switch(COMMANDMSG(&msg)->cmd)
                        {
                        case IDB_CANCEL:
                           //Get text from entry field and store.
                           lRetLen=WinQueryWindowText(
                                        WinWindowFromID(hwnd,ID_FINDTEXT),
                                        sizeof(szGlobalSearchString),
                                        &szGlobalSearchString);

 
                           //See if case-sensitive box is checked, if
                           //so, set ulCaseSensitive.
                           if(WinQueryButtonCheckstate(hwnd,ID_CASESENSITIVE))
                              {
                              ulGlobalCaseSensitive=MLFSEARCH_CASESENSITIVE;
                              }
                           else 
                              {  
                              ulGlobalCaseSensitive=0;
                              }

                           //End the dialog
                           bCentered=FALSE;
                           WinDismissDlg(hwnd,TRUE);
                           return 0;

                        case IDB_OK:
                           //Get text from entry field into buffer.
                           lRetLen=WinQueryWindowText(
                                        WinWindowFromID(hwnd,ID_FINDTEXT),
                                        sizeof(szSearchString),
                                        &szSearchString);

                           //Copy search string into global storage.
                           strcpy(szGlobalSearchString, szSearchString);

                          //Make sure something was entered to search for.  
                          if(szGlobalSearchString[0]==0)
                            {
                            WinMessageBox(HWND_DESKTOP,hwnd,
                                "You must enter something to find.",
                                "Find",
                                0,
                                MB_OK|MB_ICONEXCLAMATION|MB_MOVEABLE);

                            WinSetFocus(HWND_DESKTOP,
                                          WinWindowFromID(hwnd,ID_FINDTEXT));
                            return 0;
                            }
        

                           mlesearch.cchFind=0;
                           mlesearch.cchReplace=0;
 
                           //See if case-sensitive box is checked, if
                           //so, set ulCaseSensitive.
                           if(WinQueryButtonCheckstate(hwnd,ID_CASESENSITIVE))
                              {
                              ulCaseSensitive=MLFSEARCH_CASESENSITIVE;
                              }
                           else 
                              {  
                              ulCaseSensitive=0;
                              }
                           //Store case sensitive
                           ulGlobalCaseSensitive=ulCaseSensitive;


                           //Send message to MLE to search
                           mres=WinSendMsg
                                (hwndMLE,
                                MLM_SEARCH,
                                MPFROMLONG(MLFSEARCH_SELECTMATCH|ulCaseSensitive),
                                MPFROMP(&mlesearch ));
 
                           //If the text was not start search from beginning.
                                
                           if(!mres)
                              {
                              mlesearch.iptStart=0;
                              mres=WinSendMsg
                                (hwndMLE,
                                MLM_SEARCH,
                                MPFROMLONG(MLFSEARCH_SELECTMATCH|ulCaseSensitive),
                                MPFROMP(&mlesearch ));
                              }

                           //If text still was not found, display message.
                           if(!mres)
                              {
                              WinMessageBox(HWND_DESKTOP,hwndClient,"Text not found.",
                                            "Find",0,MB_MOVEABLE|MB_SYSTEMMODAL);
                              //Set focus back to find dialog
                              WinSetActiveWindow(HWND_DESKTOP,hwnd);
                              //Set focus to entry field  
                              WinSetFocus(HWND_DESKTOP,
                                            WinWindowFromID(hwnd,ID_FINDTEXT));
                              //Reselect the text
                              WinSendMsg(WinWindowFromID(hwnd,ID_FINDTEXT), EM_SETSEL,
                                        MPFROMLONG(0x0FFF0000),MPVOID);
  
                              }
                           else 
                              {
                              WinDismissDlg(hwnd,TRUE);
                              bCentered=FALSE;
                              }

                        return 0;
 

                        }
        break;
        }
return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

