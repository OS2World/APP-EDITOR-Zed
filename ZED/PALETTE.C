////////////////////////////////////////////////////
//PALETTE.C
//Created:	02/04/1995	Bruce Shankle
//This file contains routines to start font palette
//and color palette


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
#define PALETTE_MAIN
//Include files.
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <process.h>
#include "zed.h"
#include "palette.h"


void FontPalProc(void)
{

hFontPalette=WinQueryObject("<WP_FNTPAL>");
if(hFontPalette==NULLHANDLE)
        WinMessageBox(HWND_DESKTOP,hwndClient,
        "Could not open Font Palette",
        szAppName,0,MB_OK|MB_MOVEABLE|MB_ICONEXCLAMATION);
else
WinSetObjectData(hFontPalette,"OPEN=DEFAULT");
hwndFontPalette=WinQueryActiveWindow(HWND_DESKTOP);
}

void ColorPalProc(void)
{
char szColors[255];

hColorPalette=WinQueryObject("<WP_LORESCLRPAL>");
if(hColorPalette==NULLHANDLE)
        WinMessageBox(HWND_DESKTOP,hwndClient,
        "Could not open Color Palette",
        szAppName,0,MB_OK|MB_MOVEABLE|MB_ICONEXCLAMATION);
else
    {
    WinSetObjectData(hColorPalette,"OPEN=DEFAULT");
    strcpy(szColors,"COLORS=");
    strcat(szColors,"0xFFFFFF,");//1
    strcat(szColors,"0xFF0000,");//2
    strcat(szColors,"0xFF00FF,");//3
    strcat(szColors,"0x0000FF,");//4
    strcat(szColors,"0x00FFFF,");//5
    strcat(szColors,"0x00FF00,");//6
    strcat(szColors,"0xFFFF00,");//7
    strcat(szColors,"0xCCCCCC,");//8
    strcat(szColors,"0x000000,");//9
    strcat(szColors,"0x800000,");//10
    strcat(szColors,"0x800080,");//11
    strcat(szColors,"0x000080,");//12
    strcat(szColors,"0x008080,");//13
    strcat(szColors,"0x008000,");//14
    strcat(szColors,"0x808000,");//15
    strcat(szColors,"0x808080");//16

    WinSetObjectData(hColorPalette,szColors);
    hwndColorPalette=WinQueryActiveWindow(HWND_DESKTOP);

    }
}
