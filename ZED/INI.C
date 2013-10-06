///////////////////////////////////////////////////////////////////////////////
//INI.C
//Copyright (C) 1995 by Bruce Shankle
//
//ZED Text Editor INI file functions
//
//Created :       01/01/1995 by Bruce Shankle
//
////////////////////////////////////////////////////////////////////////////////


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
#define INCL_DOSPROCESS

//Include files.
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <process.h>
#include "resource.h"
#include "zed.h"
#include "status.h"
#include "autondnt.h"
#include "autosave.h"
#define INI_MAIN
#include "ini.h"



char szWindowPosition[]="WindowPosition";
char szMLEBackgroundColor[]="MLEBackgroundColor";
char szMLETextColor[]="MLETextColor";
char szMLEFontAttributes[]="MLEFontAttributes";
char szMLEWordWrap[]="MLEWordWrap";
char szNumTabSpaces[]="NumTabSpaces";
char szUseTabCharacter[]="UseTabCharacter";
char szStatusWindow[]="StatusWindow";
char szRecentFile1INI[]="RecentFile1";
char szRecentFile2INI[]="RecentFile2";
char szRecentFile3INI[]="RecentFile3";
char szRecentFile4INI[]="RecentFile4";
char szRecentFile5INI[]="RecentFile5";
char szRecentFile6INI[]="RecentFile6";
char szRecentFile7INI[]="RecentFile7";
char szRecentFile8INI[]="RecentFile8";
char szRecentFile9INI[]="RecentFile9";

char szCurrentDirectoryINI[]="CurrentDirectory";
char szAutoIndent[]="AutoIndent";
char szUseAutoSaveMinutes[]="UseAutoSaveMinutes";
char szUseAutoSaveModifications[]="UseAutoSaveModifications";
char szUseAutoSavePromptForName[]="UseAutoSavePromptForName";
char szAutoSaveMinutes[]="AutoSaveMinutes";
char szAutoSaveModifications[]="AutoSaveModifications";
char szCaseSensitiveFileNames[]="CaseSensitiveFileNames";
char szFirstTimeINI[]="FirstTime";
char szFirstTime[10];



char szINIFileName[CCHMAXPATH];

//////////////////////////////////////////////
//DisplayINIError
//This function displays a message box when
//data cannot be saved in the ini file.
int DisplayINIError(void)
{
PrfCloseProfile(hIni);
WinMessageBox(HWND_DESKTOP,hwndClient,
                "Could not save setup information in ZED.INI",
                szAppName,
                0,
                MB_OK|MB_MOVEABLE|MB_ICONEXCLAMATION);
return -1;
}
///////////////////////////////////////////////

///////////////////////////////////////////////
//GetINIFileName
//This functions gets the directory from
//which the exe was started, and creates
//an ini file name based on that path.
void GetINIFileName(void)
{
PTIB ptib;      //Pointer to thread info block
PPIB ppib;      //Pointer to process info block
APIRET  rc;     //Return code
PCHAR pchar;    //pointer to character

//Get info about current thread
rc = DosGetInfoBlocks(&ptib, &ppib);

//Get name of this exe file
rc=DosQueryModuleName(ppib->pib_hmte, sizeof(szINIFileName),szINIFileName);

//Set a pointer to end of filename.
pchar=szINIFileName+sizeof(szINIFileName);

//Back up until we hit a "\"
while((*pchar!='\\') && (pchar!=szINIFileName)) pchar--;

//Move forward one and set a null
if(*pchar=='\\')
        {
        pchar++;
        *pchar='\0';
        }
else
        {
        //Else this is not a path, so delete it
        strcpy(szINIFileName,"");
        }

strcat(szINIFileName,"ZED.INI");

//WinMessageBox(HWND_DESKTOP,hwndClient,szINIFileName,"ZED INI Filename",0,NULL);

}
//////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
//SaveProfile
//This function saves all profile information in
//the INI file.
//This function returns 1 if successful, otherwise it returns 0.

int SaveProfile (void)
{
SWP swp; //For getting window coordinates

//Open ini file
hIni=PrfOpenProfile(hab,szINIFileName);

//Save Window position
//Get window coordinates
WinQueryWindowPos(hwndFrame, &swp);

//Don't save window coordinates if window is minimized
if(!(swp.fl&SWP_MINIMIZE))
        {
        WinQueryWindowRect(hwndFrame, (PRECTL) &INIrcl);

        //Make coordinates relative to desktop.
        WinMapWindowPoints(hwndFrame, HWND_DESKTOP,(PPOINTL)&INIrcl,1);

        //Write the data out to the ini file
        if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szWindowPosition,
                        (PVOID) &INIrcl,
                        sizeof(RECTL)))) return DisplayINIError();

        }

//Save MLE background color
//Get background color
INIBackColor=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_QUERYBACKCOLOR,
                        MPVOID,MPVOID));
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szMLEBackgroundColor,
                        (PVOID) &INIBackColor,
                        sizeof(LONG)))) return DisplayINIError();

//Save MLE text color
INITextColor=LONGFROMMR(WinSendMsg(hwndMLE,
                        MLM_QUERYTEXTCOLOR,
                        MPVOID,MPVOID));
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szMLETextColor,
                        (PVOID) &INITextColor,
                        sizeof(LONG)))) return DisplayINIError();

//Save MLE font attributes
WinSendMsg(hwndMLE,
           MLM_QUERYFONT,
           MPFROMP((PFATTRS) &INIfattrs),
           MPVOID);
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szMLEFontAttributes,
                        (PFATTRS) &INIfattrs,
                        sizeof(FATTRS)))) return DisplayINIError();

//Save MLE word wrap status
INIbWordWrap=WinIsMenuItemChecked(hwndMenu,IDM_SETWRAP);
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szMLEWordWrap,
                        (PVOID) &INIbWordWrap,
                        sizeof(BOOL)))) return DisplayINIError();

//Save Tab information
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szNumTabSpaces,
                        (PVOID) &iGlobalNumTabSpaces,
                        sizeof(int)))) return DisplayINIError();

//Save Tab information usage
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szUseTabCharacter,
                        (PVOID) &iGlobalUseTabCharacter,
                        sizeof(int)))) return DisplayINIError();

//Save status window setting
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szStatusWindow,
                        (PVOID) &bStatusBarOn,
                        sizeof(long)))) return DisplayINIError();

//Save AutoIndent status
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szAutoIndent,
                        (PVOID) &bUseAutoIndent,
                        sizeof(BOOL)))) return DisplayINIError();

//Save recent file 1
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szRecentFile1INI,
                        szRecentFile1))) return DisplayINIError();

//Save recent file 2
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szRecentFile2INI,
                        szRecentFile2))) return DisplayINIError();

//Save recent file 3
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szRecentFile3INI,
                        szRecentFile3))) return DisplayINIError();

//Save recent file 4
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szRecentFile4INI,
                        szRecentFile4))) return DisplayINIError();

//Save recent file 5
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szRecentFile5INI,
                        szRecentFile5))) return DisplayINIError();

//Save recent file 6
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szRecentFile6INI,
                        szRecentFile6))) return DisplayINIError();

//Save recent file 7
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szRecentFile7INI,
                        szRecentFile7))) return DisplayINIError();

//Save recent file 8
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szRecentFile8INI,
                        szRecentFile8))) return DisplayINIError();

//Save recent file 9
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szRecentFile9INI,
                        szRecentFile9))) return DisplayINIError();

//Save current directory
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szCurrentDirectoryINI,
                        szCurrentDirectory))) return DisplayINIError();

//Save first time data
if(!(PrfWriteProfileString(hIni,
                        szAppName,
                        szFirstTimeINI,
                        "ZED"))) return DisplayINIError();

//Save gbUseAutoSaveMinutes
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szUseAutoSaveMinutes,
                        (PVOID) &gbUseAutoSaveMinutes,
                        sizeof(BOOL)))) return DisplayINIError();

//Save gbUseAutoSaveModifications
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szUseAutoSaveModifications,
                        (PVOID) &gbUseAutoSaveModifications,
                        sizeof(BOOL)))) return DisplayINIError();

//Save gbUseAutoSavePromptForName
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szUseAutoSavePromptForName,
                        (PVOID) &gbUseAutoSavePromptForName,
                        sizeof(BOOL)))) return DisplayINIError();

//Save gbCaseSensitiveFileNames
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szCaseSensitiveFileNames,
                        (PVOID) &gbCaseSensitiveFileNames,
                        sizeof(BOOL)))) return DisplayINIError();

//Save number of minutes for autosave
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szAutoSaveMinutes,
                        (PVOID) &glAutoSaveMinutes,
                        sizeof(glAutoSaveMinutes)))) return DisplayINIError();

//Save number of modifications for autosave
if(!(PrfWriteProfileData(hIni,
                        szAppName,
                        szAutoSaveModifications,
                        (PVOID) &glAutoSaveModifications,
                        sizeof(glAutoSaveModifications)))) return DisplayINIError();
//Close ini file
PrfCloseProfile(hIni);

//return
return 1;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//GetProfile 
//This functions retreives information from
BOOL GetProfile()
{
ULONG ulSize;

//Open INI file
hIni=PrfOpenProfile(hab,szINIFileName);

//Error check
if(hIni==NULLHANDLE) 
        {
        PrfCloseProfile(hIni);
        return FALSE;
        }


//Get window rectangle.
ulSize=sizeof(RECTL);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szWindowPosition,
                        (PVOID) &INIrcl,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get MLE background color
ulSize=sizeof(LONG);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szMLEBackgroundColor,
                        (PVOID) &INIBackColor,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;
//Get MLE text color
ulSize=sizeof(LONG);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szMLETextColor,
                        (PVOID) &INITextColor,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get MLE font attributes
ulSize=sizeof(FATTRS);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szMLEFontAttributes,
                        (PVOID) &INIfattrs,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get word wrap status
ulSize=sizeof(BOOL);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szMLEWordWrap,
                        (PVOID) &INIbWordWrap,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get tab information
ulSize=sizeof(int);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szNumTabSpaces,
                        (PVOID) &iGlobalNumTabSpaces,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get tab use information
ulSize=sizeof(int);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szUseTabCharacter,
                        (PVOID) &iGlobalUseTabCharacter,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get status bar on or off
ulSize=sizeof(bStatusBarOn);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szStatusWindow,
                        (PVOID) &bStatusBarOn,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get AutoIndent status
ulSize=sizeof(bUseAutoIndent);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szAutoIndent,
                        (PVOID) &bUseAutoIndent,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;



//Get recent file name 1
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szRecentFile1INI,
                        "",
                        szRecentFile1,
                        CCHMAXPATH);
//Get recent file name 2
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szRecentFile2INI,
                        "",
                        szRecentFile2,
                        CCHMAXPATH);
//Get recent file name 3
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szRecentFile3INI,
                        "",
                        szRecentFile3,
                        CCHMAXPATH);
//Get recent file name 4
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szRecentFile4INI,
                        "",
                        szRecentFile4,
                        CCHMAXPATH);
//Get recent file name 5
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szRecentFile5INI,
                        "",
                        szRecentFile5,
                        CCHMAXPATH);
//Get recent file name 6
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szRecentFile6INI,
                        "",
                        szRecentFile6,
                        CCHMAXPATH);
//Get recent file name 7
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szRecentFile7INI,
                        "",
                        szRecentFile7,
                        CCHMAXPATH);
//Get recent file name 8
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szRecentFile8INI,
                        "",
                        szRecentFile8,
                        CCHMAXPATH);
//Get recent file name 9
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szRecentFile9INI,
                        "",
                        szRecentFile9,
                        CCHMAXPATH);

//Get current directory
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szCurrentDirectoryINI,
                        "",
                        szCurrentDirectory,
                        sizeof(szCurrentDirectory));

////////////////////////////////////////
//Get first time run info
ulSize=PrfQueryProfileString(hIni,
                        szAppName,
                        szFirstTimeINI,
                        "",
                        szFirstTime,
                        sizeof(szFirstTime));

if (ulSize<3) goto GETPROFILEERROR;
//////////////////////////////////////

//Get gbUseAutoSaveMinutes
ulSize=sizeof(gbUseAutoSaveMinutes);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szUseAutoSaveMinutes,
                        (PVOID) &gbUseAutoSaveMinutes,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get gbUseAutoSaveModifications
ulSize=sizeof(gbUseAutoSaveModifications);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szUseAutoSaveModifications,
                        (PVOID) &gbUseAutoSaveModifications,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get gbUseAutoSavePromptForName
ulSize=sizeof(gbUseAutoSavePromptForName);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szUseAutoSavePromptForName,
                        (PVOID) &gbUseAutoSavePromptForName,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;


//Get gbCaseSensitiveFileNames
ulSize=sizeof(gbCaseSensitiveFileNames);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szCaseSensitiveFileNames,
                        (PVOID) &gbCaseSensitiveFileNames,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get glAutoSaveMinutes
ulSize=sizeof(glAutoSaveMinutes);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szAutoSaveMinutes,
                        (PVOID) &glAutoSaveMinutes,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Get glAutoSaveModifications
ulSize=sizeof(glAutoSaveModifications);
if(!(PrfQueryProfileData(hIni,
                        szAppName,
                        szAutoSaveModifications,
                        (PVOID) &glAutoSaveModifications,
                        (PULONG) &ulSize))) goto GETPROFILEERROR;

//Close INI file
PrfCloseProfile(hIni);
return TRUE;

GETPROFILEERROR:

PrfCloseProfile(hIni);
return FALSE;

}


