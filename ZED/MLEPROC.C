////////////////////////////////////////////////////////////////////////////
//MLEPROC.C
//
//This file contains the subclassed MLE procedure.
//MLECustomProc
//This procecure handles messages that the MLE normally would handle.
//If this procedure does not handle them, it will pass them to the
//original MLE procedure
//
//Created       :       01/26/1995      Bruce Shankle
//Preprocessor directive defines.
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
#define INCL_WINCLIPBOARD
#define INCL_GPI
#define INCL_WINWORKPLACE

//Include files.
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <process.h>

#include "zed.h"
#include "find.h"
#include "about2.h"
#include "replace.h"
#include "tab.h"
#include "resource.h"
#include "status.h"
#include "ini.h"
#include "palette.h"
#include "rcntfile.h"
#include "autondnt.h"
#include "autosave.h"
#include "help.h"
#include "colorchg.h"

MRESULT EXPENTRY MLECustomProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
SHORT sScrollBarFirst;
SHORT sScrollBarLast;
static SHORT sScrollBarRange;
USHORT idVScroll;
USHORT idHScroll;
static int iFirstVScroll=0;
static int iFirstHScroll=0;
static HWND hwndHScroll;
static HWND hwndVScroll;
MRESULT mres;
IPT ipt;                //Insertion point
IPT iptLineStart;       //First insertion point for a line
IPT iptTotalLineLength; //Total lenght of a line of text
IPT iptCursorToEndLength; //Length from cursor to end of line.
static long lCurrentLine;
static long lCurrentColumn;
int iRes;
static BOOL bEnterKeyDown;
LONG lColor;

switch(msg)
        {

        case MLM_SETFONT:
            FontPalProc();    
        break;
        case MLM_SETBACKCOLOR:

        WinDlgBox(HWND_DESKTOP, hwnd, ColorChgDlgProc,
            NULLHANDLE, IDD_COLORCHANGE, NULL);

        //Reshow color palette
        ColorPalProc();
        
        //If background color is changed, let message fall through
        if(glColorChgAction==COLORCHANGE_BACKGROUND)
            {
            //Get color of text.
            lColor=LONGFROMMR(WinSendMsg(hwndMLE, MLM_QUERYTEXTCOLOR,
                                                    MPVOID,MPVOID));
            if(lColor==mp1)
                {
                sprintf(buffer,"Text color and background color may not be the same.");
                WinMessageBox(HWND_DESKTOP, hwndClient, buffer,szAppName,0,
                                    MB_OK|MB_ICONEXCLAMATION|MB_MOVEABLE);
                return 0;
                }
            else
                {
                //Set color change flag so color is not changed again.
                glColorChgAction=COLORCHANGE_CANCEL;

                break;
                }
            }

        //If text color is changed, change text color and return
        if(glColorChgAction==COLORCHANGE_TEXT)
            {
            //Get color of background.
            lColor=LONGFROMMR(WinSendMsg(hwndMLE, MLM_QUERYBACKCOLOR,
                                                    MPVOID,MPVOID));
            if(lColor==mp1)
                {
                sprintf(buffer,"Text color and background color may not be the same.");
                WinMessageBox(HWND_DESKTOP, hwndClient, buffer,szAppName,0,
                                    MB_OK|MB_ICONEXCLAMATION|MB_MOVEABLE);
                return 0;
                }
            else
                {
                WinSendMsg(hwndMLE, MLM_SETTEXTCOLOR, mp1, MPVOID);
                //Set color change flag so color is not changed again.
                glColorChgAction=COLORCHANGE_CANCEL;
                return 0;
                }
            
            }
        
        //Handle cancel
        if(glColorChgAction==COLORCHANGE_CANCEL) return 0;

        
        break; //MLM_SETBACKCOLOR

        case WM_CHAR:

             //Set enter key flag down for autoindent.   
             if(!(CHARMSG(&msg)->fs & KC_KEYUP))
                {
                switch(CHARMSG(&msg)->vkey)
                    {
                    case VK_ENTER:
                    case VK_NEWLINE:
                    bEnterKeyDown=TRUE;
                    break;
                    }                    
                }

             //Let autosave know there was a modification
              if(CHARMSG(&msg)->fs & KC_KEYUP)
                {
                iRes=AutoSaveModProc(AUTOSAVEMOD_INCREMENT);
                }  

             //Handle key status changes - caps lock, num lock, insert
             if(CHARMSG(&msg)->fs & KC_KEYUP)
             if(CHARMSG(&msg)->fs & KC_VIRTUALKEY)
                switch(CHARMSG(&msg)->vkey)
                {

                case VK_PAGEUP:
                break;
                case VK_UP:
                break;

                case VK_ENTER:
                case VK_NEWLINE:
                    if(bEnterKeyDown)
                    {
                        if(bUseAutoIndent)
                        iRes=AutoIndentProc();
                    }
                bEnterKeyDown=FALSE;    
                break;
                

                case VK_INSERT:
                case VK_CAPSLOCK:
                case VK_NUMLOCK:
                        if (bStatusBarOn) UpdateKeyStatus(); //in status.c
                break;
                }
                

        case WM_BUTTON1CLICK:
                if (bStatusBarOn)
                {

                //Update line display in status window
                ipt=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_QUERYSEL,
                        MPFROMSHORT(MLFQS_CURSORSEL),
                        MPVOID));
                lGlobalCurrentLine=LONGFROMMR(WinSendMsg(hwndMLE,
                                                MLM_LINEFROMCHAR,
                                                MPFROMLONG(ipt),
                                                MPVOID))+1;
                //Get start of current line
                iptLineStart=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_CHARFROMLINE,
                        MPFROMLONG(-1L),
                        //MPFROMLONG(lGlobalCurrentLine),
                        MPVOID));

                //Get total length of line
                iptTotalLineLength=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_QUERYLINELENGTH,
                        MPFROMLONG(iptLineStart),
                        MPVOID));
 
                //Get length from cursor to end of line
                iptCursorToEndLength=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_QUERYLINELENGTH,
                        MPFROMLONG(-1L),
                        MPVOID));

                //Calculate current column
                lGlobalCurrentColumn=iptTotalLineLength-iptCursorToEndLength+1;
 
                //Update status window if line or column has changed
                if (
                   (lCurrentColumn!=lGlobalCurrentColumn) ||
                   (lCurrentLine!=lGlobalCurrentLine)
                   ) UpdateStatusWindow();

                //store curren line and column
                lCurrentColumn=lGlobalCurrentColumn;
                lCurrentLine=lGlobalCurrentLine;

                } //end if bStatusBarOn

        break;

        /////////////////////////////////////////////////////
        //Real-time vertical scrolling
        case WM_VSCROLL:

            if(iFirstVScroll==0)
                {
                //Get scroll bar ID
                idVScroll=SHORT1FROMMP(mp1);

                //Get handle of scroll vertical scroll bar.
                hwndVScroll=WinWindowFromID(hwndMLE,idVScroll);
 
                //Get scroll bar range.
                mres=WinSendMsg(hwndVScroll,
                        SBM_QUERYRANGE,
                        MPVOID,MPVOID);

                sScrollBarFirst=SHORT1FROMMR(mres);
                sScrollBarLast=SHORT2FROMMR(mres);
                sScrollBarRange=sScrollBarLast-sScrollBarFirst;

                iFirstVScroll=1;

                }

            if (SHORT2FROMMP(mp2)==SB_ENDSCROLL) break;

            switch (SHORT2FROMMP(mp2))
                {

                case SB_SLIDERTRACK:
                //Send message to fool MLE into thinking scroll has ended
                WinSendMsg(hwndMLE,
                        WM_VSCROLL,
                        mp1,
                        MPFROMLONG(
                                MAKELONG(
                                            SHORT1FROMMP(mp2),
                                            SB_SLIDERPOSITION
                                            )
                                        ));

                WinSendMsg(hwndMLE,
                        WM_VSCROLL,
                        mp1,
                        MPFROMLONG(
                                MAKELONG(
                                            SHORT1FROMMP(mp2),
                                            SB_ENDSCROLL
                                            )
                                        ));

                    
                break;


                 }

        //Invalidate scroll paint area.
        WinInvalidateRect(hwndVScroll,NULL,TRUE);

        //Force update of scroll bar
        WinUpdateWindow(hwndVScroll);

        break;//WM_VSCROLL
        /////////////////////////////////////////////////////

        /////////////////////////////////////////////////////
        //Real-time horizontal scrolling.
        case WM_HSCROLL:
            if(iFirstHScroll==0)
                {
                //Get scroll bar ID
                idHScroll=SHORT1FROMMP(mp1);

                //Get handle of scroll vertical scroll bar.
                hwndHScroll=WinWindowFromID(hwndMLE,idHScroll);
 
                iFirstHScroll=1;
    
                }

            if (SHORT2FROMMP(mp2)==SB_ENDSCROLL) break;
            

            switch (SHORT2FROMMP(mp2))
                {
                case SB_SLIDERTRACK:
                //Send message to fool MLE into thinking scroll has ended
                WinSendMsg(hwndMLE,
                         WM_HSCROLL,
                         mp1,
                        MPFROMLONG(
                                MAKELONG(
                                            SHORT1FROMMP(mp2),
                                            SB_SLIDERPOSITION
                                            )
                                        ));

                WinSendMsg(hwndMLE,
                         WM_HSCROLL,
                         mp1,
                        MPFROMLONG(
                                MAKELONG(
                                            SHORT1FROMMP(mp2),
                                            SB_ENDSCROLL
                                            )
                                        ));
                    
                break;
                }
        //Invalidate scroll paint area.
        WinInvalidateRect(hwndHScroll,NULL,TRUE);

        //Force update of scroll bar
        WinUpdateWindow(hwndHScroll);

            
        break; //WM_HSCROLL
        /////////////////////////////////////////////////////

        }

return MLEDefProc(hwnd,msg,mp1,mp2);
}
///////////////////////////////////////////////////////////////////////////
