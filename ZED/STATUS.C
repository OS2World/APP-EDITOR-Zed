//////////////////////////////////////////////////////////////////
//STATUS.C
//Functions for status window
//Created:       01/29/1995      Bruce Shankle
//////////////////////////////////////////////////////////////////

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
#define STATUS_DOT_C


//Include files.
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <process.h>
#include "zed.h"
#include "easyfont.h"
#include "status.h"
#include "resource.h"
#include "autosave.h"


#define LCID_MYFONT 1L //Font ID
#define ID_STATUS_TIMER 1 //Timer ID



//Ln Col 3D box dimensions
#define LINECOLBOX_LEFT 2
#define LINECOLBOX_BOTTOM 2
#define LINECOLBOX_TOP 18
#define LINECOLBOX_RIGHT 150

//INS/OVR 3d box dimensions
#define INSOVR_LEFT 152
#define INSOVR_BOTTOM 2
#define INSOVR_TOP 18
#define INSOVR_RIGHT 190

//CAPS lock 3d box
#define CAPSL_LEFT 192
#define CAPSL_BOTTOM 2
#define CAPSL_TOP 18
#define CAPSL_RIGHT 230

//NUM lock 3d box
#define NUML_LEFT 232
#define NUML_BOTTOM 2
#define NUML_TOP 18
#define NUML_RIGHT 270

//MODIFIED 3d box
#define MOD_LEFT 272
#define MOD_BOTTOM 2
#define MOD_TOP 18
#define MOD_RIGHT 351

//TIME 3d box
#define TIME_LEFT 353
#define TIME_BOTTOM 2
#define TIME_TOP 18
#define TIME_RIGHT 440

//Installed date box
#define INST_LEFT 442
#define INST_BOTTOM 2
#define INST_TOP 18
#define INST_RIGHT 630



//Status button text
char cInsertText[5];
char cNumLockText[5];
char cCapsLockText[5];
char cModifiedText[10];
char cTimeText[10];

void CreateStatusWindow(void)
{
RECTL rcl;

//Register class for status window
WinRegisterClass(
                hab,
                "ZEDStatus",
                StatusWndProc,
                CS_SIZEREDRAW|CS_SYNCPAINT,
                0);

//Get client window coordinates
WinQueryWindowRect(hwndClient, &rcl);

//Create status window
hwndStatus= WinCreateWindow(
                hwndClient,             //Owner window handle
                "ZEDStatus",              //Window Class
                (PSZ)NULL,              //Control data
                WS_VISIBLE,             //Style
                rcl.xLeft,              //X1 coordinate
                rcl.yBottom,            //Y1 coordinate
                rcl.xRight,             //X2 coordinate
                rcl.yBottom+STATUSWND_HEIGHT,               //Y2 coordinate
                hwndClient,             //Parent window handle
                HWND_TOP,               //Style
                ID_STATUS,                 //Identifier
                NULL,                   //Control data
                NULL);                  //Presentation parameters

//Start timer for status window Timer box
WinStartTimer(hab, hwndStatus, ID_STATUS_TIMER, 1000);



}

///////////////////////////////////////
//Window procedure for status window
MRESULT EXPENTRY StatusWndProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
HPS hps;
RECTL rcl;
char szbuf[20];
int iRes;
switch(msg)
        {
        case WM_CREATE:
                //Initialize fonts
                hps=WinGetPS(hwnd);
                EzfQueryFonts(hps);
                WinReleasePS(hps);
                bModified=FALSE;

                //Initialize key status
                UpdateKeyStatus();

                //Initialize time info
                StatusTimerProc();

                //Check Status Bar menuitem
                WinCheckMenuItem(hwndMenu, IDM_STATUSBAR, TRUE);


 
        return 0;

        case WM_PAINT:
 
                //Don't do any painting if status bar
                //is off
                if(!bStatusBarOn) 
                {
                hps=WinBeginPaint(hwnd,NULLHANDLE,NULL);
                WinEndPaint(hps);
                return 0;
                }

                hps=WinBeginPaint(hwnd,NULLHANDLE,NULL);
                WinQueryWindowRect(hwnd,&rcl);

                //Fill window with gray
                WinFillRect(hps, &rcl, CLR_PALEGRAY);

                //Draw 3d box for line column information
                Draw3DBox(LINECOLBOX_LEFT,
                          LINECOLBOX_BOTTOM ,
                          LINECOLBOX_RIGHT,
                          LINECOLBOX_TOP,
                          hps);

                //Draw 3d box for INS/OVR status
                Draw3DBox(INSOVR_LEFT,INSOVR_BOTTOM,
                          INSOVR_RIGHT, INSOVR_TOP,
                          hps);

                //Draw 3d box for Caps lock
                Draw3DBox(CAPSL_LEFT, CAPSL_BOTTOM,
                          CAPSL_RIGHT, CAPSL_TOP,
                          hps);
 
                //Draw 3d box for num lock
                Draw3DBox(NUML_LEFT, NUML_BOTTOM,
                          NUML_RIGHT, NUML_TOP,
                          hps);

                //Draw 3d box for "MODIFIED"
                Draw3DBox(MOD_LEFT, MOD_BOTTOM,
                          MOD_RIGHT, MOD_TOP,
                          hps);

                //Draw 3d box for time
                Draw3DBox(TIME_LEFT, TIME_BOTTOM,
                          TIME_RIGHT, TIME_TOP,
                          hps);

                //Draw 3d box for install date
                Draw3DBox(INST_LEFT, INST_BOTTOM, 
                          INST_RIGHT, INST_TOP,
                          hps);
                

                //Draw insert text
                DrawString(INSOVR_LEFT+2,
                           INSOVR_BOTTOM+3,
                           hps,
                           cInsertText);

                //Draw caps lock text
                DrawString(CAPSL_LEFT+2,
                           CAPSL_BOTTOM+3,
                           hps,
                           cCapsLockText);

                //Draw num lock text
                DrawString(NUML_LEFT+2,
                           NUML_BOTTOM+3,
                           hps,
                           cNumLockText);
 
                //Draw text for MODIFIED
                DrawString(MOD_LEFT+2,
                           MOD_BOTTOM+3,
                           hps,
                           cModifiedText);

                //Draw in text for install date
                DrawString(INST_LEFT+2,
                            INST_BOTTOM+3,
                            hps,
                            szInstInfo);

                //Draw time text
                DrawString(TIME_LEFT+2,
                           TIME_BOTTOM+3,
                           hps, cTimeText);

                //Update status ln X col x string
                sprintf(szbuf,"Ln %d Col %d",lGlobalCurrentLine,lGlobalCurrentColumn);
                DrawString(LINECOLBOX_LEFT+2,
                           LINECOLBOX_BOTTOM+3,
                           hps,
                           szbuf);



                WinEndPaint(hps);


        return 0; //WM_PAINT

        case WM_SIZE:

        return 0;

        case WM_TIMER:
                //Update time on status bar.
                StatusTimerProc();

                //Send message to autosave process to increment it's value.
                iRes=AutoSaveTimerProc(AUTOSAVETIMER_INCREMENT);
        return 0;

        case WM_CLOSE:
                //Destroy the timer
                WinStopTimer(hab, hwndStatus, ID_STATUS_TIMER);
                //Destroy the window
                WinDestroyWindow(hwnd);
        return 0;//WM_CLOSE
        }
return WinDefWindowProc(hwnd,msg,mp1,mp2);
}

////////////////////////////////////////////////////////
//Draw3DBox
//Created       01/29/1995      Bruce Shankle

void Draw3DBox(long x1, long y1, long x2, long y2, HPS hps)
{
POINTL ptl;

////////Draw bottom.
GpiSetColor(hps,CLR_WHITE);
ptl.x=x1;
ptl.y=y1;
GpiMove(hps,&ptl);

//Set up end of line
ptl.x=x2;
GpiLine(hps,&ptl);

////////Draw right
ptl.y=y2;
GpiLine(hps,&ptl);

////////Draw top
GpiSetColor(hps,CLR_DARKGRAY);
ptl.x=x1;
GpiLine(hps, &ptl);

////////Draw left
ptl.y=y1;
GpiLine(hps,&ptl);


}





/////////////////////////////////////////////////////////////
//DrawString
//Created       01/29/1995       Bruce Shankle
void DrawString(long x, long y, HPS hps, char * szString)
{
POINTL ptl;
//Get font
if(EzfCreateLogFont(
        hps,
        LCID_MYFONT,
        FONTFACE_COUR,
        FONTSIZE_8,
        0))
        {
        GpiSetColor(hps,CLR_BLACK);
        ptl.x=x;
        ptl.y=y;
        GpiSetCharSet(hps, LCID_MYFONT);
        GpiMove(hps, &ptl);
        GpiCharString(hps,strlen(szString),szString);
        GpiSetCharSet(hps, LCID_DEFAULT);
        GpiDeleteSetId(hps, LCID_MYFONT);
        }
}



////////////////////////////////////////////////////
//UpdateStatusWindow
void UpdateStatusWindow(void)
{
RECTL rcl;
                
rcl.xLeft=LINECOLBOX_LEFT;
rcl.xRight=LINECOLBOX_RIGHT;
rcl.yTop=LINECOLBOX_TOP;
rcl.yBottom=LINECOLBOX_BOTTOM;
//WinInvalidateRect(hwndStatus,NULL,TRUE);
WinInvalidateRect(hwndStatus,&rcl,TRUE);

}





/////////////////////////////////////////////////
//ResetLineColumnStatus
void ResetLineColumnStatus(void)
{
lGlobalCurrentLine=1;
lGlobalCurrentColumn=1;
UpdateStatusWindow();
}


///////////////////////////////////////////////////////
//UpdateKeyStatus
//This function determines the state of the insert key
void UpdateKeyStatus (void)
{

//char szbuf[80];
//sprintf(szbuf,"%d",bCapsLock);
//WinMessageBox(HWND_DESKTOP,hwndClient,szbuf,"",0,NULL);

//Get states of keys
bInsert=WinQuerySysValue(HWND_DESKTOP, SV_INSERTMODE);
//bInsert=(WinGetKeyState(HWND_DESKTOP, VK_INSERT) & 0x0001);
bCapsLock=(WinGetKeyState(HWND_DESKTOP,VK_CAPSLOCK) & 0x0001);
bNumLock=(WinGetKeyState(HWND_DESKTOP,VK_NUMLOCK) & 0x0001);


//Update what is show in the staus window for the keys
if(bInsert) sprintf(cInsertText,"INS");
else sprintf(cInsertText,"OVR");

if(bCapsLock) sprintf(cCapsLockText,"CAPS");
else sprintf(cCapsLockText," ");

if(bNumLock) sprintf(cNumLockText,"NUM");
else sprintf(cNumLockText," ");

if(bModified) sprintf(cModifiedText,"MODIFIED");
else sprintf(cModifiedText," ");


WinInvalidateRect(hwndStatus,NULL,TRUE);
}

//////////////////////////////////////////////////
//StatusTimerProc
//Updates time in staus window
void StatusTimerProc(void)
{
DATETIME dt;
RECTL rcl;
static DATETIME olddt;

//Get current time
DosGetDateTime(&dt);
sprintf(cTimeText,
        "%d%s%02d %cM",
       (dt.hours+11) % 12 + 1,
        ":",
        dt.minutes,
        dt.hours / 12 ? 'P' : 'A');

//Only repaint screen when minutes change.
if (olddt.minutes != dt.minutes)
        {
        //Invalidate time box
        rcl.xLeft=TIME_LEFT;
        rcl.xRight=TIME_RIGHT;
        rcl.yTop=TIME_TOP;
        rcl.yBottom=TIME_BOTTOM;
        WinInvalidateRect(hwndStatus,&rcl,TRUE);
        olddt.minutes=dt.minutes;
        }

}

void TurnStatusBarOn(void)
{
RECTL rcl;

//Set Flag
bStatusBarOn=TRUE;

//Get client size
WinQueryWindowRect(hwndClient, &rcl);

//Reposition MLE
WinSetWindowPos(
        hwndMLE,
        HWND_TOP,
        rcl.xLeft,
        rcl.yBottom+STATUSWND_HEIGHT,
        rcl.xRight,
        rcl.yTop-STATUSWND_HEIGHT,
        SWP_SIZE|SWP_MOVE|SWP_SHOW);

//Force repaint of status bar
WinShowWindow(hwndStatus,TRUE);

//Update status window
UpdateKeyStatus();



}

void TurnStatusBarOff(void)
{

RECTL rcl;

//Set flag
bStatusBarOn=FALSE;

//Hide status window
WinShowWindow(hwndStatus,FALSE);

//Get client size
WinQueryWindowRect(hwndClient, &rcl);

//Reposition MLE window
WinSetWindowPos(
        hwndMLE,
        HWND_TOP,
        rcl.xLeft,
        rcl.yBottom,
        rcl.xRight,
        rcl.yTop,
        SWP_SIZE|SWP_MOVE|SWP_SHOW);


}

