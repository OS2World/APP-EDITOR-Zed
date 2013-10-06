///////////////////////////////////////////////////////////
// CHKFILES.C
// Routines to check and make sure files exist.
///////////////////////////////////////////////////////////


#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS
#define INCL_WINWORKPLACE
#define INCL_DOS
#include <os2.h>
#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <time.h>

#include "zin.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////
//CheckFiles
// Makes sure all needed install files exist.
// Returns 0 if successful.

int CheckFiles(char * szDir)
{
int iRes;

CHAR szCurrentDir[CCHMAXPATH];
CHAR szDirAndFile[CCHMAXPATH+12];
CHAR szFileName[LITTLEBUF];
CHAR szMessage[MAXBUF];

//Copy directory
strcpy(szCurrentDir, szDir);

//Check ZED.EXE
SetStatus(ID_ZEDEXE);
WinLoadString(hab, 0, ID_ZED_FILENAME, LITTLEBUF, szFileName);
sprintf(szDirAndFile, "%s%s", szCurrentDir, szFileName);
iRes=access(szDirAndFile, R_OK);
if (iRes!=0) goto ERROR;

//Check ZED.HLP
SetStatus(ID_ZEDHLP);
WinLoadString(hab, 0, ID_ZED_HELP_FILENAME, LITTLEBUF, szFileName);
sprintf(szDirAndFile, "%s%s", szCurrentDir, szFileName);
iRes=access(szDirAndFile, R_OK);
if (iRes!=0) goto ERROR;

//Check ZED.ICO
SetStatus(ID_ZEDICO);
WinLoadString(hab, 0, ID_ZED_ICON_FILENAME, LITTLEBUF, szFileName);
sprintf(szDirAndFile, "%s%s", szCurrentDir, szFileName);
iRes=access(szDirAndFile, R_OK);
if (iRes!=0) goto ERROR;

//Check README.TXT
SetStatus(ID_ZEDREADME);
WinLoadString(hab, 0, ID_ZED_README_FILENAME, LITTLEBUF, szFileName);
sprintf(szDirAndFile, "%s%s", szCurrentDir, szFileName);
iRes=access(szDirAndFile, R_OK);
if (iRes!=0) goto ERROR;

//Check REGISTER.TXT
//SetStatus(ID_REGISTER);
//WinLoadString(hab, 0, ID_REGISTER_FILENAME, LITTLEBUF, szFileName);
//sprintf(szDirAndFile, "%s%s", szCurrentDir, szFileName);
//iRes=access(szDirAndFile, R_OK);
//if (iRes!=0) return -1;


return 0;

ERROR:
    sprintf(szMessage,"%s %s",
        "Can't find the file ", szDirAndFile);
    WinMessageBox(HWND_DESKTOP, hwndClient, szMessage,
                        "ZED - Install", 0, 
                        MB_OK | MB_MOVEABLE | MB_ICONHAND);
    return -1;

}


int CopyFiles(char * szSourceDirectory, char * szDestinationDirectory)
{

ULONG opmode;
APIRET apiret;
CHAR szSrcDir[CCHMAXPATH];
CHAR szDestDir[CCHMAXPATH];
CHAR szSrcDirAndFile[CCHMAXPATH+12];
CHAR szDestDirAndFile[CCHMAXPATH+12];
CHAR szFileName[LITTLEBUF];


opmode=DCPY_EXISTING|DCPY_FAILEAS;

//Copy source directory
strcpy(szSrcDir, szSourceDirectory);

//Copy dest dir
strcpy(szDestDir, szDestinationDirectory);

//Copy ZED.EXE
SetStatus(ID_COPYZEDEXE);
WinLoadString(hab, 0, ID_ZED_FILENAME, LITTLEBUF, szFileName);
sprintf(szSrcDirAndFile, "%s%s", szSrcDir, szFileName);
sprintf(szDestDirAndFile, "%s\\%s", szDestDir, szFileName);
apiret=DosCopy(szSrcDirAndFile, szDestDirAndFile, opmode);
if (apiret!=0) return -1;

//Copy ZED.HLP
SetStatus(ID_COPYZEDHLP);
WinLoadString(hab, 0, ID_ZED_HELP_FILENAME, LITTLEBUF, szFileName);
sprintf(szSrcDirAndFile, "%s%s", szSrcDir, szFileName);
sprintf(szDestDirAndFile, "%s\\%s", szDestDir, szFileName);
apiret=DosCopy(szSrcDirAndFile, szDestDirAndFile, opmode);
if (apiret!=0) return -1;


//Copy ZED.ICO
SetStatus(ID_COPYZEDICO);
WinLoadString(hab, 0, ID_ZED_ICON_FILENAME, LITTLEBUF, szFileName);
sprintf(szSrcDirAndFile, "%s%s", szSrcDir, szFileName);
sprintf(szDestDirAndFile, "%s\\%s", szDestDir, szFileName);
apiret=DosCopy(szSrcDirAndFile, szDestDirAndFile, opmode);
if (apiret!=0) return -1;


//Copy README.TXT
SetStatus(ID_COPYZEDREADME);
WinLoadString(hab, 0, ID_ZED_README_FILENAME, LITTLEBUF, szFileName);
sprintf(szSrcDirAndFile, "%s%s", szSrcDir, szFileName);
sprintf(szDestDirAndFile, "%s\\%s", szDestDir, szFileName);
apiret=DosCopy(szSrcDirAndFile, szDestDirAndFile, opmode);
if (apiret!=0) return -1;


//Copy REGISTER.TXT
//SetStatus(ID_COPYREGISTER);
//WinLoadString(hab, 0, ID_REGISTER_FILENAME, LITTLEBUF, szFileName);
//sprintf(szSrcDirAndFile, "%s%s", szSrcDir, szFileName);
//sprintf(szDestDirAndFile, "%s\\%s", szDestDir, szFileName);
//apiret=DosCopy(szSrcDirAndFile, szDestDirAndFile, opmode);
//if (apiret!=0) return -1;

return 0;
}

int CreateObject(char * szDestDir)
{
CHAR szDestination[CCHMAXPATH];
CHAR szSetupString[MAXBUF];


SetStatus(ID_CREATINGOBJECT);
strcpy(szDestination, szDestDir);
sprintf(szSetupString,"EXENAME=%s\\ZED.EXE", szDestination);
strcat(szSetupString,";PROGTYPE=PM;ICONFILE=");
strcat(szSetupString,szDestination);
strcat(szSetupString,"\\ZED.ICO");
if (WinCreateObject("WPProgram",
                    "ZED",
                    szSetupString,
                    "<WP_DESKTOP>",
                    CO_REPLACEIFEXISTS)) return 0;

/*if (SysCreateObject("WPProgram", ,
                  "ZED Test 1.0s", ,
                  "<WP_DESKTOP>", ,
                  szSetupString, ,
                  "REPLACE")) return 0;*/
return -1;
}

int PatchZed(char * szDestDir)
{

struct tm tmbuf;
time_t time_of_day;
CHAR szZED[CCHMAXPATH];
CHAR szDDIR[CCHMAXPATH];
CHAR szDate[10];
CHAR szTSNT[10];
FILE * infile;
int iRes;
long lPos;

//Change status
SetStatus(ID_PATCHINGZED);
strcpy(szDDIR, szDestDir);

//Get time
time_of_day=time(NULL);
_localtime(&time_of_day, &tmbuf);

//Get file name
sprintf(szZED, "%s\\ZED.EXE", szDDIR);

//Format the date
sprintf(szDate,"%d/%d/%d", 
            tmbuf.tm_mon+1,
            tmbuf.tm_mday,
            tmbuf.tm_year);

//Open ZED.EXE
infile=fopen(szZED, "r+b");
if (infile==NULL) return -1;


//Find 'TSNT'
//WinMessageBox(HWND_DESKTOP, hwndClient, "Getting ready to search.","",0,NULL);
lPos=0x15000;
while (strncmp(szTSNT, "TSNT",4)!=0)
    {
    iRes=fseek(infile, lPos, SEEK_SET);
    if (iRes!=0) goto ERROR;
    iRes=fread(szTSNT, 1, 4, infile);
    if (iRes!=4) goto ERROR;
    lPos++;
    }


//Flush the read
iRes=fflush(infile);
//if (iRes!=0) goto ERROR;

//Write date stamp to file.
//WinMessageBox(HWND_DESKTOP, hwndClient, "Getting ready to write.","",0,NULL);
iRes=fwrite(szDate, 1, strlen(szDate), infile);
if (iRes!=strlen(szDate)) goto ERROR;

//Close file
fclose(infile);

return 0;

ERROR:
    fclose(infile);
    return -1;

}


