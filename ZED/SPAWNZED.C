///////////////////////////////////////////////////////////////////////////////
//SPAWNZED.C
//Copyright (C) 1995 by Bruce Shankle
//
//This file contains the functions to start a new ZED instance.
//
//Created :       02/19/1995 by Bruce Shankle
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
#include "zed.h"
#include "spawnzed.h"

//#ifndef _RESULTCODES
//typedef struct _RESULTCODES {  /* result codes */
//    ULONG codeTerminate;      /* Termination code or process ID */
//    ULONG codeResult;         /* Exit code */
//    } RESULTCODES;
//#endif


int SpawnProc(void)
{
char szEXEName[CCHMAXPATH]; //buffer to hold filename
PTIB ptib;      //Pointer to thread info block
PPIB ppib;      //Pointer to process info block
APIRET  rc;     //Return code
CHAR        LoadError[100];
PSZ         Args;
PSZ         Envs;
RESULTCODES ReturnCodes;

//Get rid of compiler warnings for unused vars.
Args=NULL;
Envs=NULL;

//Get info about current thread
rc = DosGetInfoBlocks(&ptib, &ppib);

//Get name of this exe file
rc=DosQueryModuleName(ppib->pib_hmte, sizeof(szEXEName),szEXEName);

//WinMessageBox(HWND_DESKTOP,hwndClient,szEXEName,"",0,NULL);

//Start new instance
rc = DosExecPgm(LoadError,        /* Object name buffer */
            sizeof(LoadError),       /* Length of object name buffer */
            EXEC_ASYNC,           /* Asynchronous/Trace flags */
            Args,                    /* Argument string */
            Envs,                    /* Environment string */
            &ReturnCodes,          /* Termination codes */
            szEXEName);            /* Program file name */

if (rc!=0) 
    {
    sprintf(buffer,"Spawn failed: %s : Error code: %d, Hex: %x",
                        rc,rc,LoadError);
    WinMessageBox(HWND_DESKTOP,
                        hwndClient,
                        buffer,
                        szAppName,
                        0,
                        NULL);
                        
    }
    
return 0;
}
