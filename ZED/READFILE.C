//////////////////////////////////////////////////////////////////////////////
//READFILE.C
//ReadFile
//
//Reads a file into the MLE window.
//This process is started as a second thread
//by FileOpenProc in FILEOPEN.C
//
//Created         :       01/06/1995      Bruce Shankle
//
//Parameters       :       char * szInfile         Pointer to input file name.
//
//Notes   :
//        The handle to the MLE control is global. It is accessed here.
//
//

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

//Include files
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <process.h>
#include "zed.h"
#include "status.h"
#include "rcntfile.h"
#include "autosave.h"
#include "errormsg.h"

//Local error message box helper function
void ErrorMessage (char * szErrorMsg);

void ReadFile(char * szInfile)
{
IPT lOffset=0;
HFILE hfile;
ULONG ulAction;
static ULONG cbBytesRead;
ULONG cbBytesImported;
PVOID pvBuf;             //pointer to buffer to hold file.
static long BUFSIZE=32000;
HAB hablocal;
HMQ hmqlocal;
FILESTATUS3 FileInfoBuf;
ULONG cbTotalBytesRead;
LONG lPercentRead;
int iRes;


//Initialize anchor block
hablocal=WinInitialize(0);

//Create message queue
hmqlocal=WinCreateMsgQueue(hablocal,0);

//Clear the MLE window.
WinSetWindowText(hwndMLE,"");

//Set text limit of MLE control to unlimited
WinSendMsg(hwndMLE, MLM_SETTEXTLIMIT, MPFROMLONG(-1), MPVOID);

//Set window title to "Opening current filename ..."
sprintf(szTitleBuffer,"%s - Opening %s...",szAppName,szInfile);
WinSetWindowText(hwndFrame,szTitleBuffer);

//Open the file.
if (DosOpen(szInfile, &hfile, &ulAction,0,FILE_NORMAL,
                        FILE_OPEN,
                        OPEN_ACCESS_READONLY|OPEN_SHARE_DENYNONE,
                        NULL))
                {
                //Show error message
                ErrorMessage("Could not open file.");

                //Goto end
                goto ENDREADFILE;
                }

//Get information about the file
DosQueryFileInfo(hfile,1,&FileInfoBuf,sizeof(FileInfoBuf));
cbTotalBytesRead=0;

//Allocate a buffer.
if (DosAllocMem((PPVOID)&pvBuf, BUFSIZE, fALLOC))
//if (DosAllocMem((PPVOID)&pvBuf, BUFSIZE, PAG_READ|PAG_WRITE|PAG_GUARD|PAG_COMMIT))
{
        //Error allocating memory.
        ErrorMessage("Could not allocate memory to import file.");

        //Goto end
        goto ENDREADFILE;

}

//Set MLE import buffer.
WinSendMsg(hwndMLE, 
        MLM_SETIMPORTEXPORT,
        MPFROMP((PBYTE)pvBuf),
        MPFROMLONG(BUFSIZE));

//Disable MLE auto-refresh and hide it.
WinSendMsg(hwndMLE,
        MLM_DISABLEREFRESH,
        MPVOID,
        MPVOID);

//Make window not paint
WinShowWindow(hwndMLE,FALSE);

//Set insertion point
lOffset = 0;

//Read the file and import into MLE window
do
  {
        //Sleep a bit
        DosSleep(100);

        //Read in some of the file
        if (DosRead(hfile, pvBuf, BUFSIZE, &cbBytesRead))
        {
        //Show error message
        ErrorMessage("File read failed.");

        //An error occured
        goto ENDREADFILE;
        }


        //Accumulate number of bytes read
        cbTotalBytesRead=cbTotalBytesRead+cbBytesRead;
 
        //Calculate percentage read.
        if(FileInfoBuf.cbFile>0)
         lPercentRead=(100*cbTotalBytesRead/FileInfoBuf.cbFile);

        //Update title bar with percentage read
        sprintf(szTitleBuffer,"%s - Opening %s... %ld%%",
                        szAppName,szInfile,
                        lPercentRead);

        WinSetWindowText(hwndFrame,szTitleBuffer);

        //Import text to MLE if there were bytes read in.
        if(cbBytesRead>0)
        {
                cbBytesImported=LONGFROMMR(WinSendMsg(hwndMLE, 
                                                MLM_IMPORT,
                                                MPFROMP(&lOffset),
                                                MPFROMLONG(cbBytesRead)));

                if(!cbBytesImported)
                {
                        //Show error message
                        ErrorMessage("Text not imported.");

                        //Bytes were not imported, error
                        goto ENDREADFILE;



                }
        }


 
} while (cbBytesRead>0); //End do


//Reset title change flag.
bUpdateTitle=TRUE;

//Reset current line and column
ResetLineColumnStatus();

//Set status to not MODIFIED
bModified=FALSE;
UpdateKeyStatus();


//Set file name to current file
strcpy(szFileName, szInfile);

//Add file to recent file list
iRes=AddRecentFile(szFileName);

//Set window title to current filename
sprintf(szTitleBuffer,"%s - [%s]",szAppName,szInfile);
WinSetWindowText(hwndFrame,szTitleBuffer);

//Set the changed flag to false in MLE
WinSendMsg(hwndMLE, MLM_SETCHANGED, MPFROMSHORT((BOOL)FALSE), NULL);

//Reset autosave
iRes=AutoSaveModProc(AUTOSAVEMOD_INITIALIZE);
iRes=AutoSaveTimerProc(AUTOSAVETIMER_INITIALIZE);

//Label for end of function, errors goto here.
ENDREADFILE:

//Re-enable MLE auto refresh and show it
WinSendMsg(hwndMLE, MLM_ENABLEREFRESH, MPVOID, MPVOID);
WinShowWindow(hwndMLE,TRUE);

EnableFileMenu(TRUE);

//Close the file
DosClose(hfile);

//Free the memory
DosFreeMem(pvBuf);

//Destroy this threads message queue
WinDestroyMsgQueue(hmqlocal);

//Destroy local anchor block
WinTerminate(hablocal);

//Set flag for status of this thread
iReadFileFinished=1;

//End this thread
_endthread();

}
//End ReadFile

