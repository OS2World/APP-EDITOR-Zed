///////////////////////////////////////////////////////////////
//HELP.C
//Functions for online help.
//Created:     02/20/1995    Bruce Shankle

//Preprocessor directive defines.
#define INCL_WINSTDFILE
#define INCL_WINWINDOWMGR
#define INCL_WINFRAMEMGR
#define INCL_WINMLE
#define INCL_WINSYS
#define INCL_WINERRORS
#define INCL_WIN
#define INCL_WINCLIPBOARD
#define INCL_WINWORKPLACE
#define INCL_WINHELP
#define HELP_MAIN

//Include files.
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <process.h>
#include "zed.h"
#include "help.h"
#include "resource.h"
#include "autosave.h"


//////////////////////////////////////////////////
//InitializeHelp
//This function initalizes a help instance for the program
int InitializeHelp(void)
{
int iRes;
char szHelpFileName[CCHMAXPATH];

//Setup help structure
hiInit.cb=sizeof(HELPINIT);
hiInit.ulReturnCode=0L;
hiInit.pszTutorialName=NULL;
hiInit.phtHelpTable=(PHELPTABLE)MAKEULONG(HELP_CLIENT,0xFFFF);
hiInit.hmodHelpTableModule=NULLHANDLE;
hiInit.hmodAccelActionBarModule=NULLHANDLE;
hiInit.idAccelTable=0;
hiInit.idActionBar=0;
hiInit.pszHelpWindowTitle="ZED Help";
hiInit.fShowPanelId=CMIC_HIDE_PANEL_ID;


strcpy(szHelpFileName,"");
GetDirectory(szHelpFileName);
strcat(szHelpFileName,"ZED.HLP");
hiInit.pszHelpLibraryName=szHelpFileName;

iRes=access(szHelpFileName,F_OK);
if (iRes==-1) 
    {
    strcpy(szHelpFileName,"ZED.HLP");
    }

//Create help instance
hwndHelp=WinCreateHelpInstance(hab,&hiInit);
if ((hwndHelp!=NULLHANDLE) && (hiInit.ulReturnCode!=0))
    {
    sprintf(buffer,"Could not create help instance. Help will be disabled.");
    WinMessageBox(HWND_DESKTOP,hwndClient,buffer,szAppName,0,
                            MB_OK|MB_INFORMATION|MB_MOVEABLE);
    iRes=DisableHelp();
    WinDestroyHelpInstance(hwndHelp);
    hwndHelp=NULLHANDLE;
    return -1;
    }

if (hwndHelp==NULLHANDLE) 
    {
    sprintf(buffer,"Could not find help file ZED.HLP. Help will be disabled.");
    WinMessageBox(HWND_DESKTOP,hwndClient,buffer,szAppName,0,
                            MB_OK|MB_INFORMATION|MB_MOVEABLE);
    iRes=DisableHelp();
    return -1;
    }


//Associate help instance with main window
WinAssociateHelpInstance(hwndHelp,hwndFrame);

//Install hook
WinSetHook (hab, hmq, HK_HELP, (PFN) helpHook, NULLHANDLE) ;


return 0;
}
/////////////////////////////////////////////

/////////////////////////////////////////////
//DestroyHelp
//This functions destroys the help instance.
int DestroyHelp(void)
{

if (hwndHelp!=(HWND)NULL)
    {
    //Release help hook
    WinReleaseHook ( hab,hmq,HK_HELP,(PFN) helpHook,NULLHANDLE) ;

    //Unassociate help instance
    WinAssociateHelpInstance (NULLHANDLE, hwndFrame ) ;

    //Destroy help instance
    WinDestroyHelpInstance(hwndHelp);
    hwndHelp=NULLHANDLE;
    }

return 0;
}
////////////////////////////////////////////

////////////////////////////////////////////
//General help functions

//Auto-save help
void HelpAutoSave(void)
{
if (hwndHelp!=(HWND)NULL)
    {
    WinSendMsg ( hwndHelp,
                    HM_DISPLAY_HELP,
                    MPFROMLONG ( MAKELONG ( HTOPIC_AUTOSAVE, 0 ) ) ,
                    MPFROMSHORT ( HM_RESOURCEID ) ) ;

    }
}


//Help-using-help help
void HelpUsingHelp(void)
{
if (hwndHelp!=(HWND)NULL)
    {   
    WinSendMsg(hwndHelp,HM_DISPLAY_HELP,MPVOID,MPVOID);
    }
}

//Help-index
void HelpIndex(void)
{
if (hwndHelp!=(HWND)NULL)
    {
    WinSendMsg (hwndHelp,HM_HELP_INDEX,MPVOID,MPVOID) ;
    }
}

//Help contents
void HelpContents(void)
{
if (hwndHelp!=(HWND)NULL)
    {
    WinSendMsg ( hwndHelp,
                    HM_HELP_CONTENTS,
                    MPVOID,
                    MPVOID) ;

    }
}

//Help General help
void HelpGeneral(void)
{
if (hwndHelp!=(HWND)NULL)
    {
    WinSendMsg (hwndHelp,HM_EXT_HELP,MPVOID,MPVOID) ;
    }
}

//Help keys help
void HelpKeys(void)
{
if (hwndHelp!=(HWND)NULL)
    {
    WinSendMsg ( hwndHelp,
                    HM_DISPLAY_HELP,
                    MPFROMLONG ( MAKELONG ( IDM_KEYSHELP, 0 ) ) ,
                    MPFROMSHORT ( HM_RESOURCEID ) ) ;

    }
}

//Help first time- This function is called from ZED.C if 
//bUseIniSettings is false.
void HelpFirstTime(void)
{
if (hwndHelp!=(HWND)NULL)
    {
    
    sprintf(buffer,"Thank-you for trying ZED.\n");
    strcat(buffer,"Please take a moment to read the registration information in the online help.\n");


    WinMessageBox(HWND_DESKTOP, hwndClient, buffer,"ZED - Registration Information",
                        0,
                        MB_OK|MB_MOVEABLE); 

    WinSendMsg ( hwndHelp,
                    HM_DISPLAY_HELP,
                    MPFROMLONG ( MAKELONG ( HTOPIC_COPYRIGHT, 0 ) ) ,
                    MPFROMSHORT ( HM_RESOURCEID ) ) ;


    }
}

//Help colors help
void HelpColors(void)
{
if (hwndHelp!=(HWND)NULL)
    {
    WinSendMsg ( hwndHelp,
                    HM_DISPLAY_HELP,
                    MPFROMLONG ( MAKELONG ( HTOPIC_COLORS, 0 ) ) ,
                    MPFROMSHORT ( HM_RESOURCEID ) ) ;

    }
}

//Help tabs help
void HelpTabs(void)
{
if (hwndHelp!=(HWND)NULL)
    {
    WinSendMsg ( hwndHelp,
                    HM_DISPLAY_HELP,
                    MPFROMLONG ( MAKELONG ( HTOPIC_TABS, 0 ) ) ,
                    MPFROMSHORT ( HM_RESOURCEID ) ) ;

    }
}

/////////////////////////////////////////////


/////////////////////////////////////////////
//helpHook
//This function handles help messages from
//message boxes that have a Help button
BOOL EXPENTRY helpHook ( HAB habAnchor,
                          SHORT sMode,
                          USHORT usTopic,
                          USHORT usSubTopic,
                          PRECTL prclPos )
{
//Avoid warnings
habAnchor=habAnchor;
prclPos=prclPos;
usSubTopic=usSubTopic;

if ( ( sMode == HLPM_WINDOW ) && ( hwndHelp != NULLHANDLE ))
    {
    WinSendMsg ( hwndHelp,
                    HM_DISPLAY_HELP,
                    MPFROMLONG ( MAKELONG ( usTopic, 0 ) ) ,
                    MPFROMSHORT ( HM_RESOURCEID ) ) ;
    return TRUE ;
    } 
else
    {
    return FALSE ;
    }
 }
////////////////////////////////////////////////////

////////////////////////////////////////////////////
//DisableHelp
//This function disables the help menu items that refer
//to ZED.HLP
int DisableHelp(void)
{
WinEnableMenuItem(hwndMenu, IDM_HELPINDEX, FALSE);
WinEnableMenuItem(hwndMenu, IDM_HELPCONTENTS, FALSE);
WinEnableMenuItem(hwndMenu, IDM_GENERALHELP, FALSE);
WinEnableMenuItem(hwndMenu, IDM_USINGHELP, FALSE);
WinEnableMenuItem(hwndMenu, IDM_KEYSHELP, FALSE);
return 0;
}
////////////////////////////////////////////////////
