//////////////////////////////////////////////////////////////////////////////
//AUTOSAVE.C
//
//
//Created :       02/14/1994      Bruce Shankle
//
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
#define AUTOSAVE_MAIN
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <io.h>

#include "zed.h"
#include "autosave.h"
#include "help.h"

///////////////////////////////////////////////////////////////////////
//AutoSaveFile
//This function is called from AutoSaveTimerProc to save the current file.

int AutoSaveFile(void)
{
BOOL bTextChanged;
int iRes;
int iLoopCtr;
char szWindowTitle[256];
char szASFileName[CCHMAXPATH];
char szTempFileName[CCHMAXPATH];

//See if text has changed.
bTextChanged=LONGFROMMR(WinSendMsg(
                                hwndMLE,
                                MLM_QUERYCHANGED,
                                MPVOID,
                                MPVOID));

//If text has not changed then return
if (!bTextChanged) return 0;

//Get window title
WinQueryWindowText(hwndFrame,sizeof(szWindowTitle),szWindowTitle);

//Change title to autosave
sprintf(buffer,"%s - AutoSaving...",szAppName);
WinSetWindowText(hwndFrame,buffer);

//If file already has a name save it
if(szFileName[0]!=0)
        {
        iRes=SaveFile(szFileName);
        if(iRes!=0)
                {
                WinMessageBox(HWND_DESKTOP,hwndClient,
                        "Warning - AutoSave failed.",
                        szAppName,
                        HTOPIC_AUTOSAVE,
                        MB_HELP|MB_OK|MB_MOVEABLE|MB_ICONEXCLAMATION);
                WinSetWindowText(hwndFrame,szWindowTitle);
                }
        return 0;
        }


//Prompt for name?
if ( (szFileName[0]==0) && (gbUseAutoSavePromptForName) )
        {
        strcpy(buffer,"AutoSave needs to save the current text.\n");
        strcat(buffer,"Please supply a filename.");
        WinMessageBox(HWND_DESKTOP,hwndClient,
                            buffer,
                            szAppName,
                            HTOPIC_AUTOSAVEWARNING1,
                            MB_OK|MB_MOVEABLE|MB_HELP);
        FileSaveAsProc(hwndClient);
        //Does the file have a name yet?
        if(szFileName[0]==0)
                {
                strcpy(buffer,"AutoSave has detected this file has still not been named.\n");
                strcat(buffer,"With the current settings, AutoSave will continue to request\n");
                strcat(buffer,"a filename each time it runs until the file is named.");
                WinMessageBox(HWND_DESKTOP,hwndClient,
                                    buffer,
                                    szAppName,
                                    HTOPIC_AUTOSAVEWARNING2,
                                    MB_OK|
                                    MB_MOVEABLE|
                                    MB_INFORMATION|
                                    MB_HELP);
                 WinSetWindowText(hwndFrame,szWindowTitle);   
                 }//endif szFileName[0]==0

        return 0;
        }

//Auto-assign file name and save.
GetDirectory(szTempFileName);
strcat(szTempFileName,"AUTOSAVE.");

//Find next available file extension. Extensions range from 001 to 999
for (iLoopCtr=1; iLoopCtr<=999; iLoopCtr++)
    {
    sprintf(szASFileName,"%s%03d",szTempFileName,iLoopCtr);
    iRes=access(szASFileName,F_OK);
    if (iRes==-1) break;
    }

//Was a file found?
if(iRes!=-1)
    {
    //Give warning.
    sprintf(buffer,"AutoSave failed. Too many \n%sXXX files",szTempFileName);
    WinMessageBox(HWND_DESKTOP,
                    hwndClient,
                    buffer,
                    "ZED AutoSave",
                    HTOPIC_AUTOSAVE,
                    MB_OK|
                    MB_MOVEABLE|
                    MB_ICONEXCLAMATION|
                    MB_HELP);
    WinSetWindowText(hwndFrame,szWindowTitle);
    return 0;    
    }

//Store file name
strcpy(szFileName,szASFileName);

//Save file
iRes=SaveFile(szFileName);

//Make sure it saved.
if(iRes!=0)
        {
        WinMessageBox(HWND_DESKTOP,hwndClient,
                            "Warning - AutoSave failed.",
                            szAppName,
                            HTOPIC_AUTOSAVE,
                            MB_OK|
                            MB_MOVEABLE|
                            MB_ICONEXCLAMATION|
                            MB_HELP);
        WinSetWindowText(hwndFrame,szWindowTitle);   
        return 0;
        }//endif iRes!=0
    
//Title needs to updated now.
sprintf(szTitleBuffer,"%s - [%s]",szAppName,szFileName);
WinSetWindowText(hwndFrame,szTitleBuffer);
bUpdateTitle=TRUE;

return 0;
}

//End AutoSaveFile
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
//AutoSaveTimerProc
//This function handles auto-save on per time basis.
//It is called from STATUS.C's WM_TIMER message.

int AutoSaveTimerProc(int Action)
{
static long lSeconds;
int iRes;


//Initialize timer?
if(Action==AUTOSAVETIMER_INITIALIZE)
    {
    lSeconds=0;
    return 0;
    }

//If AutoSave is not used based on time return
if(!gbUseAutoSaveMinutes) return 0;

//Increment timer?
if(Action==AUTOSAVETIMER_INCREMENT)
    {
    //Increment counter.
    lSeconds++;

    //Time to save?
    if(lSeconds==(60*glAutoSaveMinutes)) 
        {
        iRes=AutoSaveFile();
        lSeconds=0;
        }
    }

return 0;
}

//End AutoSaveTimerProc
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
//AutoSaveModProc
//This function handles auto-save on per modification.

int AutoSaveModProc(int Action)
{
static long lModifications;
int iRes;

//Initialize timer?
if(Action==AUTOSAVEMOD_INITIALIZE)
    {
    lModifications=0;
    return 0;
    }

//If AutoSave is not used based on time return
if(!gbUseAutoSaveModifications) return 0;

//Increment counter?
if(Action==AUTOSAVEMOD_INCREMENT)
    {
    //Increment counter.
    lModifications++;

    //Time to save?
    if(lModifications==(glAutoSaveModifications)) 
        {
        iRes=AutoSaveFile();
        lModifications=0;
        }
    }

return 0;
}

//End AutoSaveModProc
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
//AutoSaveDlgProc
//This function handls the AutoSave settings dialog

MRESULT EXPENTRY AutoSaveDlgProc (HWND hwnd, 
                                        ULONG msg,
                                        MPARAM mp1, 
                                        MPARAM mp2)
{
SHORT ix, iy;
SHORT iwidth, idepth;
SWP   swp;
static BOOL bCentered=FALSE;

//Window handles
static HWND hwndCHKMinutes;
static HWND hwndCHKModifications;
static HWND hwndSPINMinutes;
static HWND hwndSPINModifications;
static HWND hwndCHKPromptForName;

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
                //Get window handles
                hwndCHKMinutes=WinWindowFromID(hwnd,
                                        ID_CHK_AUTOSAVEMINUTES);
                hwndCHKModifications=WinWindowFromID(hwnd,
                                        ID_CHK_AUTOSAVEMODIFICATIONS);
                hwndCHKPromptForName=WinWindowFromID(hwnd,
                                        ID_CHK_AUTOSAVEPROMPTFORNAME);
                hwndSPINMinutes=WinWindowFromID(hwnd,
                                        ID_SPIN_AUTOSAVEMINUTES);
                hwndSPINModifications=WinWindowFromID(hwnd,
                                        ID_SPIN_AUTOSAVEMODIFICATIONS);

                
                //Set limits of spin button for minutes
                WinSendMsg(
                        hwndSPINMinutes,
                        SPBM_SETLIMITS,
                        MPFROMLONG(60),
                        MPFROMLONG(1));
        
                //Set limits of spin button for modifications
                WinSendMsg(
                        hwndSPINModifications,
                        SPBM_SETLIMITS,
                        MPFROMLONG(5000),
                        MPFROMLONG(1));

                //Initialize spin button value for minutes
                WinSendMsg(
                        hwndSPINMinutes,
                        SPBM_SETCURRENTVALUE,
                        MPFROMSHORT(glAutoSaveMinutes),
                        MPVOID);

                //Initialize spin button value for modifications
                WinSendMsg(
                        hwndSPINModifications,
                        SPBM_SETCURRENTVALUE,
                        MPFROMLONG(glAutoSaveModifications),
                        MPVOID);

                //Initialize check buttons
                 WinCheckButton(hwnd,
                        ID_CHK_AUTOSAVEMINUTES,
                        gbUseAutoSaveMinutes);
                 WinCheckButton(hwnd,
                        ID_CHK_AUTOSAVEMODIFICATIONS,
                        gbUseAutoSaveModifications);
                 WinCheckButton(hwnd,
                        ID_CHK_AUTOSAVEPROMPTFORNAME,
                        gbUseAutoSavePromptForName);

                //Enable or disable spin buttons
                 WinEnableWindow(hwndSPINMinutes,
                                    gbUseAutoSaveMinutes);

                 WinEnableWindow(hwndSPINModifications,
                                    gbUseAutoSaveModifications);

                //Set focus to dialog
                WinSetFocus(HWND_DESKTOP,hwnd);
                            
        return MPFROMSHORT(1); //WM_INITDLG (Return 1 to force repaint.)

        case WM_CONTROL:
            switch(SHORT1FROMMP(mp1))
            {

            case ID_CHK_AUTOSAVEMINUTES:
                {
                //Enable or disable minutes spin button
                WinEnableWindow(hwndSPINMinutes,
                                        LONGFROMMR(
                                        WinSendMsg(
                                            hwndCHKMinutes,
                                            BM_QUERYCHECK,
                                            MPVOID,
                                            MPVOID)));
                break;
                }

            case ID_CHK_AUTOSAVEMODIFICATIONS:
                {
                //Enable or disable modifications spin button
                WinEnableWindow(hwndSPINModifications,
                                        LONGFROMMR(
                                        WinSendMsg(
                                            hwndCHKModifications,
                                            BM_QUERYCHECK,
                                            MPVOID,
                                            MPVOID)));
                break;
                }
            }
        return 0; //WM_CONTROL

        case WM_COMMAND:
                switch(COMMANDMSG(&msg)->cmd)
                        {
                        case IDB_AUTOSAVEOK:
            
                        //Get check button state for using minutes
                        gbUseAutoSaveMinutes=LONGFROMMR(WinSendMsg(
                                                hwndCHKMinutes,
                                                BM_QUERYCHECK,
                                                MPVOID,
                                                MPVOID));

                        //Get check button state for using modifications
                        gbUseAutoSaveModifications=LONGFROMMR(WinSendMsg(
                                                hwndCHKModifications,
                                                BM_QUERYCHECK,
                                                MPVOID,
                                                MPVOID));

                        //Get check button state for using prompt-for-name
                        gbUseAutoSavePromptForName=LONGFROMMR(WinSendMsg(
                                                hwndCHKPromptForName,
                                                BM_QUERYCHECK,
                                                MPVOID,
                                                MPVOID));

                         //Get spin value for minutes
                         WinSendMsg(hwndSPINMinutes,
                                        SPBM_QUERYVALUE,
                                        MPFROMP(&glAutoSaveMinutes),
                                        MPFROMLONG(0L));

                         //Get spin value for modifications
                         WinSendMsg(hwndSPINModifications,
                                        SPBM_QUERYVALUE,
                                        MPFROMP(&glAutoSaveModifications),
                                        MPFROMLONG(0L));
                    
                          bCentered=FALSE;
                          WinDismissDlg(hwnd,TRUE);
                          return 0; //IDB_AUTOSAVEOK

                        case IDB_AUTOSAVECANCEL:
                            bCentered=FALSE;
                            WinDismissDlg(hwnd,TRUE);
                            
                        return 0;//IDB_AUTOSAVECANCEL
                        
                        case IDB_AUTOSAVEHELP:
                            HelpAutoSave();
                        return 0; //IDB_AUTOSAVEHELP

                        }
        break;
        }
return WinDefDlgProc(hwnd, msg, mp1, mp2);
}
//End AutoSaveDlgProc                                           
////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
//GetDirectory
//This functions gets the directory ZED was started in.
void GetDirectory(char * szDir)
{
PTIB ptib;      //Pointer to thread info block
PPIB ppib;      //Pointer to process info block
APIRET  rc;     //Return code
PCHAR pchar;    //pointer to character
char szEXEName[CCHMAXPATH];

//Get info about current thread
rc = DosGetInfoBlocks(&ptib, &ppib);

//Get name of this exe file
rc=DosQueryModuleName(ppib->pib_hmte, sizeof(szEXEName),szEXEName);

//Set a pointer to end of filename.
pchar=szEXEName + ( sizeof ( szEXEName ) );

//Back up until we hit a "\"
while ( ( *pchar != '\\' ) && ( pchar != szEXEName ) ) pchar=pchar-1;

//Move forward one and set a null
if ( (*pchar) == ( '\\' ) )
        {
        //WinMessageBox(HWND_DESKTOP,hwndClient,szEXEName,"1",0,NULL);
        //Set null at end of string.
        pchar++;
        *pchar='\0';
        //WinMessageBox(HWND_DESKTOP,hwndClient,szEXEName,"2",0,NULL);
        }
else
        {
        //Else this is not a path, so delete it
        strcpy(szEXEName,"C:\\");
        }

strcat(szEXEName,"");
strcpy(szDir,"");
strcat(szDir,szEXEName);
}
