//////////////////////////////////////////////////////////////////////////////
//FINDNEXT.C
//FindNextProc
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

void FindNextProc (void)
{
MLE_SEARCHDATA mlesearch;
MRESULT mres;

//Set up search structure
mlesearch.cb=sizeof(mlesearch);
mlesearch.pchFind=szGlobalSearchString;
mlesearch.pchReplace=NULL;
mlesearch.iptStart=-1;
mlesearch.iptStop=-1;

//Send search message to MLE window.
mres=WinSendMsg
        (hwndMLE,
        MLM_SEARCH,
        MPFROMLONG(MLFSEARCH_SELECTMATCH|ulGlobalCaseSensitive),
        MPFROMP(&mlesearch ));
 

if(!mres)
   {
   mlesearch.iptStart=0;
   mres=WinSendMsg
        (hwndMLE,
        MLM_SEARCH,
        MPFROMLONG(MLFSEARCH_SELECTMATCH|ulGlobalCaseSensitive),
        MPFROMP(&mlesearch ));

   }
//If the text was not found display a message.
if(!mres)
        WinMessageBox(HWND_DESKTOP,hwndClient,"Text not found.",
        "Find",0,MB_MOVEABLE|MB_SYSTEMMODAL);


}

