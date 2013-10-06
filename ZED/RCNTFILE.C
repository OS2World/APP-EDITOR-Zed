///////////////////////////////////////////////////////////////////////////////
//RCNTFILE.C
//Copyright (C) 1995 by Bruce Shankle
//
//ZED Text Recent file functions
//
//Created :       02/11/1995 by Bruce Shankle
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
#include "ini.h"

char szLocalFilename[CCHMAXPATH];

//////////////////////////////////////////////////////////////
//AddRecentFile
//
//This function adds a filename to the recent file list or
//brings it to the top of the list.
int AddRecentFile(char * szFilename)
{
strcpy(szLocalFilename,szFilename);

//What file is this?
//WinMessageBox(HWND_DESKTOP, hwndClient,szLocalFilename,"AddRecentFile",0,NULL);
//Is file blank?
if(szFilename[0]==0) return 0;

//Is file already in list?
if (strncmp(szLocalFilename,szRecentFile1,sizeof(szRecentFile1))==0)
    return(BringToTop(szFilename));
if (strncmp(szLocalFilename,szRecentFile2,sizeof(szRecentFile2))==0)
    return(BringToTop(szFilename));
if (strncmp(szLocalFilename,szRecentFile3,sizeof(szRecentFile3))==0)
    return(BringToTop(szFilename));
if (strncmp(szLocalFilename,szRecentFile4,sizeof(szRecentFile4))==0)
    return(BringToTop(szFilename));
if (strncmp(szLocalFilename,szRecentFile5,sizeof(szRecentFile5))==0)
    return(BringToTop(szFilename));
if (strncmp(szLocalFilename,szRecentFile6,sizeof(szRecentFile6))==0)
    return(BringToTop(szFilename));
if (strncmp(szLocalFilename,szRecentFile7,sizeof(szRecentFile7))==0)
    return(BringToTop(szFilename));
if (strncmp(szLocalFilename,szRecentFile8,sizeof(szRecentFile8))==0)
    return(BringToTop(szFilename));
if (strncmp(szLocalFilename,szRecentFile9,sizeof(szRecentFile9))==0)
    return(BringToTop(szFilename));

//Wasn't in list
return(AddToList(szFilename));
}

/////////////////////////////////////////////////////////////
//AddToList
//
//This functions adds a new time to the file list.
int AddToList(char * szFilename)
{
//What file is this?
//WinMessageBox(HWND_DESKTOP, hwndClient,szFilename,"AddToList",0,NULL);
if (szFilename[0]==0) return 0;
strcpy(szRecentFile9, szRecentFile8);
strcpy(szRecentFile8, szRecentFile7);
strcpy(szRecentFile7, szRecentFile6);
strcpy(szRecentFile6, szRecentFile5);
strcpy(szRecentFile5, szRecentFile4);
strcpy(szRecentFile4, szRecentFile3);
strcpy(szRecentFile3, szRecentFile2);
strcpy(szRecentFile2, szRecentFile1);
strcpy(szRecentFile1, szFilename);
return (UpdateRecentFileMenu());
}

/////////////////////////////////////////////////////////////
//BringToTop
//
//This function brings the specified filename to the top of the
//recent file list.
int BringToTop(char * szFilename)
{
//What file is this?
//WinMessageBox(HWND_DESKTOP, hwndClient,szFilename,"BringToTop",0,NULL);

if (szFilename[0]==0) return 0;
//Is is at top already?
if (strcmp(szFilename,szRecentFile1)==0) return 0;

//Is it second?
if (strcmp(szFilename,szRecentFile2)==0)
    {
    strcpy(szRecentFile2,szRecentFile1);
    strcpy(szRecentFile1,szFilename);
    return (UpdateRecentFileMenu());
    }

//Is it third
if (strcmp(szFilename,szRecentFile3)==0)
    {
    strcpy(szRecentFile3,szRecentFile2);
    strcpy(szRecentFile2,szRecentFile1);
    strcpy(szRecentFile1,szFilename);
    return (UpdateRecentFileMenu());
    }

//Is it fourth
if (strcmp(szFilename,szRecentFile4)==0)
    {
    strcpy(szRecentFile4, szRecentFile3);
    strcpy(szRecentFile3, szRecentFile2);
    strcpy(szRecentFile2, szRecentFile1);
    strcpy(szRecentFile1, szFilename);
    return (UpdateRecentFileMenu());
    }

//Is it fifth
if (strcmp(szFilename, szRecentFile5)==0)
    {
    strcpy(szRecentFile5, szRecentFile4);
    strcpy(szRecentFile4, szRecentFile3);
    strcpy(szRecentFile3, szRecentFile2);
    strcpy(szRecentFile2, szRecentFile1);
    strcpy(szRecentFile1, szFilename);
    return (UpdateRecentFileMenu());
    }

//Is it sixth
if (strcmp(szFilename, szRecentFile6)==0)
    {
    strcpy(szRecentFile6, szRecentFile5);
    strcpy(szRecentFile5, szRecentFile4);
    strcpy(szRecentFile4, szRecentFile3);
    strcpy(szRecentFile3, szRecentFile2);
    strcpy(szRecentFile2, szRecentFile1);
    strcpy(szRecentFile1, szFilename);
    return (UpdateRecentFileMenu());
    }

//Is it seventh
if (strcmp(szFilename, szRecentFile7)==0)
    {
    strcpy(szRecentFile7, szRecentFile6);
    strcpy(szRecentFile6, szRecentFile5);
    strcpy(szRecentFile5, szRecentFile4);
    strcpy(szRecentFile4, szRecentFile3);
    strcpy(szRecentFile3, szRecentFile2);
    strcpy(szRecentFile2, szRecentFile1);
    strcpy(szRecentFile1, szFilename);
    return (UpdateRecentFileMenu());
    }

//Is it eigth
if (strcmp(szFilename, szRecentFile8)==0)
    {
    strcpy(szRecentFile8, szRecentFile7);
    strcpy(szRecentFile7, szRecentFile6);
    strcpy(szRecentFile6, szRecentFile5);
    strcpy(szRecentFile5, szRecentFile4);
    strcpy(szRecentFile4, szRecentFile3);
    strcpy(szRecentFile3, szRecentFile2);
    strcpy(szRecentFile2, szRecentFile1);
    strcpy(szRecentFile1, szFilename);
    return (UpdateRecentFileMenu());
    }

//Is it ninth
if (strcmp(szFilename, szRecentFile9)==0)
    {
    strcpy(szRecentFile9, szRecentFile8);
    strcpy(szRecentFile8, szRecentFile7);
    strcpy(szRecentFile7, szRecentFile6);
    strcpy(szRecentFile6, szRecentFile5);
    strcpy(szRecentFile5, szRecentFile4);
    strcpy(szRecentFile4, szRecentFile3);
    strcpy(szRecentFile3, szRecentFile2);
    strcpy(szRecentFile2, szRecentFile1);
    strcpy(szRecentFile1, szFilename);
    return (UpdateRecentFileMenu());
    }

return (UpdateRecentFileMenu());
}

//////////////////////////////////////////////////////////////
//UpdateRecentFileMenu
//
//This function updates the file menu with the current recent file list
int UpdateRecentFileMenu(void)
{
MENUITEM mnuItem;
char szItemText[CCHMAXPATH+4];
HWND hwndFileSubMenu;

//Delete all menu items
WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_RECENTFILE1,TRUE)),
              MPVOID);
WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_RECENTFILE2,TRUE)),
              MPVOID);
WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_RECENTFILE3,TRUE)),
              MPVOID);
WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_RECENTFILE4,TRUE)),
              MPVOID);
WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_RECENTFILE5,TRUE)),
              MPVOID);
WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_RECENTFILE6,TRUE)),
              MPVOID);
WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_RECENTFILE7,TRUE)),
              MPVOID);
WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_RECENTFILE8,TRUE)),
              MPVOID);
WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_RECENTFILE9,TRUE)),
              MPVOID);

WinSendMsg(hwndMenu,
              MM_DELETEITEM,
              MPFROMLONG(MAKELONG(IDM_SEP2,TRUE)),
              MPVOID);

//Insert only items if they exists
if (szRecentFile1[0]==0) return 0;

//Get menu information on file menu
WinSendMsg(hwndMenu,
             MM_QUERYITEM,
             MPFROMLONG(MAKELONG(IDM_FILE,TRUE)),
             MPFROMP(&mnuItem));

hwndFileSubMenu=mnuItem.hwndSubMenu;

//Get position of SEP1
WinSendMsg(hwndFileSubMenu,
             MM_QUERYITEM,
             MPFROMLONG(MAKELONG(IDM_SEP1,TRUE)),
             MPFROMP(&mnuItem));

//Setup menu item structure.
mnuItem.afStyle=MIS_TEXT;
mnuItem.afAttribute=0;


//Insert recent file 1
mnuItem.iPosition++;
mnuItem.id=IDM_RECENTFILE1;
sprintf(szItemText,"~1 %s",szRecentFile1);
WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));

//Insert recent file 2
if(szRecentFile2[0]!=0)
    {
    mnuItem.iPosition++;
    mnuItem.id=IDM_RECENTFILE2;
    sprintf(szItemText,"~2 %s",szRecentFile2);
    WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));
    }
else goto ENDUPDATE;

//Insert recent file 3
if(szRecentFile3[0]!=0)
    {
    mnuItem.iPosition++;
    mnuItem.id=IDM_RECENTFILE3;
    sprintf(szItemText,"~3 %s",szRecentFile3);
    WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));
    }
else goto ENDUPDATE;

//Insert recent file 4
if(szRecentFile4[0]!=0)
    {
    mnuItem.iPosition++;
    mnuItem.id=IDM_RECENTFILE4;
    sprintf(szItemText,"~4 %s",szRecentFile4);
    WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));
    }
else goto ENDUPDATE;

//Insert recent file 5
if(szRecentFile5[0]!=0)
    {
    mnuItem.iPosition++;
    mnuItem.id=IDM_RECENTFILE5;
    sprintf(szItemText,"~5 %s",szRecentFile5);
    WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));
    }
else goto ENDUPDATE;

//Insert recent file 6
if(szRecentFile6[0]!=0)
    {
    mnuItem.iPosition++;
    mnuItem.id=IDM_RECENTFILE6;
    sprintf(szItemText,"~6 %s",szRecentFile6);
    WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));
    }
else goto ENDUPDATE;

//Insert recent file 7
if(szRecentFile7[0]!=0)
    {
    mnuItem.iPosition++;
    mnuItem.id=IDM_RECENTFILE7;
    sprintf(szItemText,"~7 %s",szRecentFile7);
    WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));
    }
else goto ENDUPDATE;

//Insert recent file 8
if(szRecentFile8[0]!=0)
    {
    mnuItem.iPosition++;
    mnuItem.id=IDM_RECENTFILE8;
    sprintf(szItemText,"~8 %s",szRecentFile8);
    WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));
    }
else goto ENDUPDATE;

//Insert recent file 9
if(szRecentFile9[0]!=0)
    {
    mnuItem.iPosition++;
    mnuItem.id=IDM_RECENTFILE9;
    sprintf(szItemText,"~9 %s",szRecentFile9);
    WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));
    }
else goto ENDUPDATE;

ENDUPDATE:
//Insert a separator
mnuItem.iPosition++;
mnuItem.id=IDM_SEP2;
mnuItem.afStyle=MIS_SEPARATOR;
sprintf(szItemText,"");
WinSendMsg(hwndFileSubMenu,
              MM_INSERTITEM,
              MPFROMP(&mnuItem),
              MPFROMP(szItemText));

return 0;  
}








