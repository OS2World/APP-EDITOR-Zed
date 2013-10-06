//////////////////////////////////////////////////////////////////////////////
//REPLACE.C
//ReplaceDlgProc
//
//Procedure for replace dialog box.
//Created:      01/22/1994      Bruce Shankle
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
#include "replace.h"
#include "verify.h"


MRESULT EXPENTRY ReplaceDlgProc (HWND hwnd,
                                        ULONG msg,
                                        MPARAM mp1, 
                                        MPARAM mp2)
{
char szSearchString[256];
char szReplaceString[256];
ULONG ulCaseSensitive;
MLE_SEARCHDATA mlesearch;
MRESULT mres;
IPT firstchar;
SHORT ix, iy;
SHORT iwidth, idepth;
SWP   swp;
static BOOL bCentered=FALSE;



mlesearch.cb=sizeof(mlesearch);
mlesearch.pchFind=szSearchString;
mlesearch.pchReplace=szReplaceString;
mlesearch.iptStart=0;
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
                WinCheckButton(hwnd,ID_REPLACECASESENSITIVE,TRUE);
                else
                WinCheckButton(hwnd,ID_REPLACECASESENSITIVE,FALSE);


                //If no text was selected, get text from global search string.
                if(szSearchString[0]==0)
                   strcpy(szSearchString,szGlobalSearchString);
 
                //Put the selected text into the dialog box entry field.
                WinSetWindowText(WinWindowFromID(hwnd, ID_FINDWHAT), szSearchString);
                WinSetWindowText(WinWindowFromID(hwnd, ID_REPLACEWITH),
                                  szGlobalReplaceString);

                //Make the text in find window selected.
                WinSendMsg(WinWindowFromID(hwnd,ID_FINDWHAT), EM_SETSEL,
                                        MPFROMLONG(0x0FFF0000),MPVOID);

                WinSendMsg(WinWindowFromID(hwnd,ID_REPLACEWITH), EM_SETSEL,
                                        MPFROMLONG(0x0FFF0000),MPVOID);
                                                
        return 0;
        case WM_COMMAND:
                switch(COMMANDMSG(&msg)->cmd)
                        {
                        case IDB_REPLACECANCEL:

                          //Store Case sensitive flag and store
                           if(WinQueryButtonCheckstate(hwnd,
                                ID_REPLACECASESENSITIVE))
                              ulGlobalCaseSensitive=MLFSEARCH_CASESENSITIVE;            
                           else                               
                              ulGlobalCaseSensitive=0;
                             
                           //Store find text.
                           WinQueryWindowText(
                                WinWindowFromID(hwnd,ID_FINDWHAT),
                                sizeof(szGlobalSearchString),
                                &szGlobalSearchString);
 

                          //Store replace text
                           WinQueryWindowText(
                                WinWindowFromID(hwnd,ID_REPLACEWITH),
                                sizeof(szGlobalReplaceString),
                                &szGlobalReplaceString);

                           bCentered=FALSE;
                           WinDismissDlg(hwnd,TRUE);
                           return 0;//IDB_REPLACECANCEL

                        case IDB_REPLACEALL:
                           //Get replace text.
                           WinQueryWindowText(
                                WinWindowFromID(hwnd,ID_FINDWHAT),
                                sizeof(szSearchString),
                                &szSearchString);
 
                           //Save the searchstring
                           strcpy(szGlobalSearchString, szSearchString);

                          //Set size of search text
                          mlesearch.cchFind=strlen(szSearchString);

                          //Get replace text
                           WinQueryWindowText(
                                WinWindowFromID(hwnd,ID_REPLACEWITH),
                                sizeof(szReplaceString),
                                &szReplaceString);

                          //Save the replace string
                          strcpy(szGlobalReplaceString, szReplaceString);

                          //Make sure something was entered to search for.  
                          if(szGlobalSearchString[0]==0)
                            {
                            WinMessageBox(HWND_DESKTOP,hwnd,
                                "You must enter something to find.",
                                "Replace",
                                0,
                                MB_OK|MB_ICONEXCLAMATION|MB_MOVEABLE);

                            //Set focus back to entry field
                            WinSetFocus(HWND_DESKTOP,
                                            WinWindowFromID(hwnd,ID_FINDWHAT));

                            return 0;
                            }
            

                          //Set size of replace text.
                          mlesearch.cchReplace=strlen(szReplaceString);

                          //Get Case sensitive flag
                           if(WinQueryButtonCheckstate(hwnd,
                                ID_REPLACECASESENSITIVE))
                              {

                              ulCaseSensitive=MLFSEARCH_CASESENSITIVE;
                              }
                           else 
                              {  
                              ulCaseSensitive=0;
                              }

                           //Store case sensitive flag
                           ulGlobalCaseSensitive=ulCaseSensitive;

                           //Store first visible character
                           firstchar=LONGFROMMR(WinSendMsg(
                                        hwndMLE,
                                        MLM_QUERYFIRSTCHAR,
                                        MPVOID,
                                        MPVOID));

                           //Send message to MLE to search and replace all
                           mres=WinSendMsg
                                (hwndMLE,
                                MLM_SEARCH,
                                MPFROMLONG(MLFSEARCH_SELECTMATCH|
                                           MLFSEARCH_CHANGEALL|
                                           ulCaseSensitive),
                                MPFROMP(&mlesearch ));

                          if(!mres)
                             {
                             WinMessageBox(HWND_DESKTOP,hwndClient,
                                "Text not found","Replace All",0,
                                MB_MOVEABLE|MB_OK|MB_SYSTEMMODAL);

                             //Set focus back to Replace window
                             WinSetActiveWindow(HWND_DESKTOP,hwnd);

                            //Set cursor back in replace entry field.
                            WinSetFocus(HWND_DESKTOP,
                                            WinWindowFromID(hwnd,ID_FINDWHAT));

                            //Make the text in find window selected.
                            WinSendMsg(WinWindowFromID(hwnd,ID_FINDWHAT),
                                        EM_SETSEL,
                                        MPFROMLONG(0x0FFF0000),MPVOID);

                            WinSendMsg(WinWindowFromID(hwnd,ID_REPLACEWITH),
                                        EM_SETSEL,
                                        MPFROMLONG(0x0FFF0000),MPVOID);

                             }
                          else
                             {
                             //Set first visible charater back to what it was
                             WinSendMsg(
                                hwndMLE,
                                MLM_SETFIRSTCHAR,
                                MPFROMLONG(firstchar),
                                MPVOID);
                             //Dismiss the dialog box.
                             bCentered=FALSE;
                             WinDismissDlg(hwnd,TRUE);
                             }

                          return 0;//IDB_REPLACEALL

                          case IDB_REPLACEVERIFY:

                          //Store case sensitive flag
                           if(WinQueryButtonCheckstate(hwnd,
                                ID_REPLACECASESENSITIVE))
                              ulGlobalCaseSensitive=MLFSEARCH_CASESENSITIVE;            
                           else                               
                              ulGlobalCaseSensitive=0;
                             
                           //Store search text
                           WinQueryWindowText(
                                WinWindowFromID(hwnd,ID_FINDWHAT),
                                sizeof(szGlobalSearchString),
                                &szGlobalSearchString);

                          //Make sure something was entered to search for.  
                          if(szGlobalSearchString[0]==0)
                            {
                            WinMessageBox(HWND_DESKTOP,hwnd,
                                "You must enter something to find.",
                                "Replace",
                                0,
                                MB_OK|MB_ICONEXCLAMATION|MB_MOVEABLE);

                            //Set focus back to entry field
                            WinSetFocus(HWND_DESKTOP,
                                            WinWindowFromID(hwnd,ID_FINDWHAT));

                            return 0;
                            }
 

                          //Store replace text
                           WinQueryWindowText(
                                WinWindowFromID(hwnd,ID_REPLACEWITH),
                                sizeof(szGlobalReplaceString),
                                &szGlobalReplaceString);

                          //Load the verify window
                          WinLoadDlg(HWND_DESKTOP,
                                     hwndClient,
                                     VerifyDlgProc,
                                     NULLHANDLE,
                                     IDD_VERIFY,
                                     NULL);

                          //Dismiss the dialog box.
                          bCentered=FALSE;
                          WinDismissDlg(hwnd,TRUE);

                          return 0;//IDB_REPLACEVERIFY

                        }
        break;
        }
return WinDefDlgProc(hwnd, msg, mp1, mp2);

}
