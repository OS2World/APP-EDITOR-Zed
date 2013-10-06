///////////////////////////////////////////////////////////
// CHKPATH.C
// Routines to check path and create directory.
///////////////////////////////////////////////////////////


#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS
#define INCL_DOS
#include <os2.h>
#include <direct.h>
#include <stdio.h>
#include <string.h>


#include "zin.h"

APIRET CheckPath (char * szPath)
{
FILESTATUS3  PathInfoBuf;     /* File info buffer */
ULONG        PathInfoBufSize; /* Data buffer size */

//WinMessageBox(HWND_DESKTOP, hwndClient, szPath, "CheckPath", 0, NULL);
PathInfoBufSize = sizeof(FILESTATUS3);
 
return DosQueryPathInfo(szPath, 1, &PathInfoBuf,
                             PathInfoBufSize);

}


///////////////////////////////////////////////////////////
// CreateDirectory 
// Creates the specified path.
//
// History:
//    Created:    04/10/95    Bruce Shankle
//
// Returns 0 if successful, 1 otherwise.
///////////////////////////////////////////////////////////
APIRET CreateDirectory(char * szPath)
{
static CHAR szPathBuffer[CCHMAXPATH];
static CHAR szTestPathBuffer[CCHMAXPATH];
CHAR * p;


int iRes;
APIRET apiret;

//Clear path buffer
strcpy (szPathBuffer, "");


//See if last character is a \ or a / (some people type this) and eliminate it

p=szPath+strlen(szPath)-1;
if ( (*p=='\\') || (*p=='/') )
    {
    *p='\0';
    }


p=szPath+3;
strncpy(szPathBuffer, szPath, 3);


//Loop through to each \ and build and create dirs as we go
while ( strlen(szPathBuffer) < strlen(szPath) )
    {
    if ( (*p=='\\') || (*p=='/') )
        {        
   //     WinMessageBox(HWND_DESKTOP, hwndClient, "p=92", "", 0, NULL);
        //Make sure path doesn't already exist
        strcpy(szTestPathBuffer,szPathBuffer);
        apiret=CheckPath(szTestPathBuffer);

        if (apiret!=0)
            {
      //      WinMessageBox(HWND_DESKTOP, hwndClient, szPathBuffer, "mkdir", 0, NULL);
            strcpy(szTestPathBuffer,szPathBuffer);
            iRes=mkdir(szTestPathBuffer);
         //   sprintf(szBuffer,"MKDir returned: %d", iRes);
          //  WinMessageBox(HWND_DESKTOP, hwndClient, szBuffer, "", 0, NULL);
            if (iRes!=0) return 1;
            }

        strcat(szPathBuffer, "\\");
       // WinMessageBox(HWND_DESKTOP, hwndClient, szPathBuffer, "mkdir sp", 0, NULL);

        p++;
        }

    else
        {
        //Copy next character of path
        strncat(szPathBuffer, p, 1);
        //WinMessageBox(HWND_DESKTOP, hwndClient, szPathBuffer, "mkdir", 0, NULL);
        p++;
        }
    }

//Create last part of path
strcpy(szTestPathBuffer,szPathBuffer);
apiret=CheckPath(szTestPathBuffer);
if (apiret!=0)
    {
    //WinMessageBox(HWND_DESKTOP, hwndClient, szPathBuffer, "mkdir last", 0, NULL);
    strcpy(szTestPathBuffer,szPathBuffer);
    iRes=mkdir(szTestPathBuffer);
    //sprintf(szBuffer,"MKDir returned: %d", iRes);
    //WinMessageBox(HWND_DESKTOP, hwndClient, szBuffer, "", 0, NULL);
    if (iRes!=0) return 1;
    }

//Make sure path exists and return 
strcpy(szTestPathBuffer,szPathBuffer);
return CheckPath(szTestPathBuffer);
}
///////////////////////////////////////////////////////////

///////////////////////////////////////////////
//GetCurrentDirectory
//This functions gets the directory from
//which the exe was started.
void GetCurrentDirectory(char * szCurrentDirectory)
{
PTIB ptib;      //Pointer to thread info block
PPIB ppib;      //Pointer to process info block
APIRET  rc;     //Return code
PCHAR pchar;    //pointer to character

//Get info about current thread
rc = DosGetInfoBlocks(&ptib, &ppib);

//Get name of this exe file
rc=DosQueryModuleName(ppib->pib_hmte, CCHMAXPATH, szCurrentDirectory);

//Set a pointer to end of filename.
pchar=szCurrentDirectory+CCHMAXPATH;

//Back up until we hit a "\"
while((*pchar!='\\') && (pchar!=szCurrentDirectory)) pchar--;

//Move forward one and set a null
if(*pchar=='\\')
        {
        pchar++;
        *pchar='\0';
        }
}
//////////////////////////////////////////////////////

