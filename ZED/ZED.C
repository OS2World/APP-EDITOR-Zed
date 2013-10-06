///////////////////////////////////////////////////////////////////////////////
//ZED.C
//Copyright (C) 1995 by Bruce Shankle
//
//ZED Text Editor
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
#define INCL_WINWORKPLACE

#define INCL_DEV
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_WINERRORS
#define INCL_DOSPROCESS

#define ZED_MAIN


//Include files.
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <process.h>

#include "zed.h"
#include "find.h"
#include "about2.h"
#include "replace.h"
#include "tab.h"
#include "resource.h"
#include "status.h"
#include "ini.h"
#include "palette.h"
#include "rcntfile.h"
#include "autondnt.h"
#include "autosave.h"
#include "spawnzed.h"
#include "help.h"
#include "colorchg.h"
#include "import.h"

//Printer include files
#include "prnt.h"
#include "dialog.h"
PVOID  pMem;

//Begin Main
int main ( int argc, char * argv[] )
        {                                    

        //Local variables
        RECTL rcl;              //rectangle structure.
        static CHAR szClientClass[]= "ZEDClass";       //title of application
        static ULONG flFrameFlags;                       //Frame creation flags
        QMSG qmsg;              //Message
        BOOL bTextChanged;
        USHORT mbresponse;
        int iRes;               //Return from functions
        PCHAR pc;

 
        //Set priority to high until loaded
        DosSetPriority(PRTYS_PROCESS, PRTYC_TIMECRITICAL, PRTYD_MAXIMUM,0L);

        //Initialize variables 
        iReadFileFinished=1;
        iGlobalNumTabSpaces=4;
        iGlobalUseTabCharacter=0;
        bUpdateTitle=TRUE;
        lGlobalCurrentLine=1;
        lGlobalCurrentColumn=1;
        INIbWordWrap=FALSE;
        INIBackColor=CLR_BACKGROUND;      //MLE background color
        bStatusBarOn=TRUE;                //status bar defaults to on
        bUseAutoIndent=TRUE;
        glAutoSaveMinutes=10;
        glAutoSaveModifications=1000;
        gbUseAutoSaveMinutes=TRUE;
        gbUseAutoSaveModifications=TRUE;
        gbUseAutoSavePromptForName=TRUE;
        gbCaseSensitiveFileNames=FALSE;

        //For printing.
        DosAllocMem((PPVOID)&pMem,0x1000,PAG_READ | PAG_WRITE);
        DosSubSet(pMem,DOSSUB_INIT | DOSSUB_SPARSE_OBJ,0x8000);



        //Initialize recent file list variables
        szRecentFile1[0]='\0';
        szRecentFile2[0]='\0';
        szRecentFile3[0]='\0';
        szRecentFile4[0]='\0';
       
      
        //Initialize PM program
        hab=WinInitialize(0);




        //Get INI filename
        GetINIFileName();
 
        //Get ini settings for window position
        bUseIniSettings=GetProfile();   
       
        //Create message queue
        hmq=WinCreateMsgQueue(hab,0);

        
        //Register class
        WinRegisterClass(
                        hab,            // Anchor block handle
                        szClientClass,  // name of class
                        ClientWndProc, // Window procedure for class
                        CS_SIZEREDRAW,             // Class style
                        0);             // Extra bytes to reserve

        //Assign window frame creation flags
        flFrameFlags=FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER | FCF_MINMAX |
                        FCF_TASKLIST |FCF_ICON | FCF_MENU|
                        FCF_ACCELTABLE;

        //If window postion was not in INI file then include
        //default window postions in frame flags.
        if (!bUseIniSettings)
                {
                flFrameFlags=flFrameFlags|FCF_SHELLPOSITION;
                }

        //Create the frame window
        hwndFrame=WinCreateStdWindow(
                HWND_DESKTOP,           // Owner window handle
                NULL,//WS_VISIBLE,             // Style
                &flFrameFlags,          // Pointer to control data
                szClientClass,          // Client window class name
                szAppName,              // Title bar text
                0L,                     // Style of client window
                0,                      // Module handle of resources,
                ID_RESOURCE,            // ID of resources
                &hwndClient) ;          // Pointer to client window handle

        //Check install data, don't run ZED unless it's been installed.
        WinLoadString(hab, 0, 10000, 256, (PSZ) buffer);
        pc=buffer;
        //Skip TSNT
        pc=pc+4;
        if (*pc==' ')
            {
            //Has not been installed.
            sprintf(buffer,"%s%s",
                "ZED must be installed using INSTALL.EXE.\n",
                "Please run INSTALL.EXE before attempting to run ZED.");

            WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, buffer, "ZED",0,
                            MB_MOVEABLE|MB_OK|MB_ICONHAND);

            sprintf(buffer,"%s%s",
                "Please see the file README.TXT for information on installing ZED.\n",
                "ZED will now terminate.");

            WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, buffer, "ZED",0,
                            MB_MOVEABLE|MB_OK|MB_ICONHAND);


            //Destroy window.
            WinDestroyWindow(hwndFrame);
    
            //Destroy message queue
            WinDestroyMsgQueue(hmq);

            //Terminate PM program.
            WinTerminate(hab);
        
    
            return 0;

            }
        else
            {
            //Copy install date into string
            strcpy(szInstDate, pc);
            //WinMessageBox(HWND_DESKTOP, hwndClient, szInstDate,"",0,NULL);
            sprintf(szInstInfo,"%s %s", "Installed on", szInstDate);
            }


    
       
        //If window settings were retrieved from INI then
        //Implement those settings.
        if(bUseIniSettings)
                {
                WinSetWindowPos(hwndFrame,
                                HWND_TOP,
                                INIrcl.xLeft,INIrcl.yBottom,
                                INIrcl.xRight, INIrcl.yTop,
                                SWP_SIZE|SWP_MOVE);                
                }
        
       //Get size of client window.
       WinQueryWindowRect(hwndClient, &rcl);

       //Create MLE window the same size as the client window.
       hwndMLE= WinCreateWindow(
                hwndClient,             //Owner window handle
                WC_MLE,                 //Window Class
                (PSZ)NULL,              //Control data
                MLS_HSCROLL | MLS_VSCROLL | MLS_IGNORETAB | MLS_BORDER |
                WS_VISIBLE, //Style
                rcl.xLeft,              //X1 coordinate
                rcl.yBottom+STATUSWND_HEIGHT,  //Y1 coordinate
                rcl.xRight,             //X2 coordinate
                rcl.yTop-STATUSWND_HEIGHT,               //Y2 coordinate
                hwndClient,             //Parent window handle
                HWND_TOP,               //Style
                ID_MLE,                 //Identifier
                NULL,                   //Control data
                NULL);                  //Presentation parameters

        //Check or uncheck AutoIndent menu
        if(bUseAutoIndent)
            {
            WinCheckMenuItem(hwndMenu,IDM_AUTOINDENT,TRUE);
            }

        //Activate INI settings
        if(bUseIniSettings)
        {

        //Turn on CaseSensitiveFilesNames
        if(gbCaseSensitiveFileNames)
            {
            WinCheckMenuItem(hwndMenu,
                                IDM_CASESENSITIVEFILENAMES,
                                TRUE);    
            }

        //Set MLE background color
        WinSendMsg(hwndMLE,MLM_SETBACKCOLOR,MPFROMLONG(INIBackColor),MPVOID);
        //Set MLE text color
        WinSendMsg(hwndMLE,MLM_SETTEXTCOLOR,MPFROMLONG(INITextColor),MPVOID);
        //Set MLE font
        WinSendMsg(hwndMLE,MLM_SETFONT,MPFROMP((PFATTRS)&INIfattrs),MPVOID);
        //Turn word wrap on if needed
        if(INIbWordWrap)
                        {
                         //Enable word-wrap in MLE
                           WinSendMsg(hwndMLE,
                                      MLM_SETWRAP,
                                      MPFROMLONG(TRUE),
                                      MPVOID);
                         //Make menu selection checked
                           WinCheckMenuItem(hwndMenu,
                                                IDM_SETWRAP,
                                                TRUE);
 
                        }

        //Turn status bar on or off
        if(!bStatusBarOn)
                {
                 //Turn status bar off
                 TurnStatusBarOff();
                        

                }
        }
        else
        {
        WinSendMsg(hwndMLE,MLM_SETBACKCOLOR,MPFROMLONG(CLR_BACKGROUND),MPVOID);
        }
       
        //Set color of MLE text to CLR_NEUTRAL if INI file is not used
        if(!bUseIniSettings)
        WinSendMsg(hwndMLE,MLM_SETTEXTCOLOR,MPFROMLONG(CLR_NEUTRAL), MPVOID);

        //Sub-class MLE procedure to MLECustomProc
        MLEDefProc=WinSubclassWindow(hwndMLE,(PFNWP) MLECustomProc);

        //Create status window
        CreateStatusWindow();

        //CreateStatusWindow will by default check the Status Window
        //menu item, uncheck it if needed.
        if(!bStatusBarOn) WinCheckMenuItem(hwndMenu,IDM_STATUSBAR,FALSE);
                
        //Set focus to edit window.
        WinSetFocus(HWND_DESKTOP,hwndMLE);

        //Set title of frame.
        sprintf(szTitleBuffer,"%s - [untitled]",szAppName);
        WinSetWindowText(hwndFrame,szTitleBuffer);
 
        //Set initial filename to nothing.
        strcpy(szFileName,"");

        //Set startup directory.
        if(!bUseIniSettings)
        strcpy(szCurrentDirectory,"*.*");
  
        //Show the window
        WinShowWindow(hwndFrame,TRUE);

        //Initialize help
        iRes=InitializeHelp();

        if(!bUseIniSettings) HelpFirstTime();

        //Save settings now if they were not available.
        if (!bUseIniSettings) SaveProfile();

        //Check for command line arguments.
        if (argc>1) 
           {
           iRes=FileOpenProc(hwndClient, argv[1]);
           }

        //Set priority back to normal
        DosSetPriority(PRTYS_PROCESS, PRTYC_REGULAR, PRTYD_MAXIMUM,0L);

        //Initialize autosave
        iRes=AutoSaveTimerProc(AUTOSAVETIMER_INITIALIZE);
        iRes=AutoSaveModProc(AUTOSAVEMOD_INITIALIZE);

        //Begin message loop.
        while(TRUE)
                {
                //Get and dispatch messages
                while(WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0))
                        {
                        if(!iReadFileFinished)
                                {
                                DosSuspendThread(ReadThreadID);
                                }
                        WinDispatchMsg(hab, &qmsg);
                        if(!iReadFileFinished)
                                {
                                DosResumeThread(ReadThreadID);
                                }
                        }

                //If the Read file thread is running, kill it?
                if (!iReadFileFinished)
                   {
                    //Suspend opening the file
                    DosSuspendThread(ReadThreadID);

                    //See if user wants to terminate thread.
                    sprintf(buffer,"A file is currently being opened.\n\n");
                    strcat(buffer,"Do you want kill this process?");
                        mbresponse=WinMessageBox(HWND_DESKTOP,
                                hwndClient,
                                buffer,
                                szAppName,
                                0,
                                MB_YESNOCANCEL|
                                MB_ICONQUESTION|
                                MB_MOVEABLE);
 
                    if (mbresponse==MBID_YES)
                        {
                        bTextChanged=FALSE;
                        DosKillThread(ReadThreadID);
                        }
                    else
                        {
                        DosResumeThread(ReadThreadID);
                        goto CANCELSHUTDOWN;
                        }


                   }
                else
                   {
                   //Query MLE control to see if text has changed.
                   bTextChanged=LONGFROMMR(WinSendMsg(hwndMLE,
                                MLM_QUERYCHANGED,
                                MPVOID,
                                MPVOID));
                    }
                //Verify close message if text has changed.
                if(bTextChanged)
                        {
                        sprintf(buffer,"The text in the file has changed.\n\n");
                        strcat(buffer,"Do you want to save the changes?");
                        mbresponse=WinMessageBox(HWND_DESKTOP,
                                hwndClient,
                                buffer,
                                szAppName,
                                0,
                                MB_YESNOCANCEL|
                                MB_ICONQUESTION|
                                MB_MOVEABLE);
                        if (mbresponse==MBID_NO) goto ENDPROGRAM;
                        if (mbresponse==MBID_YES)
                                {
                                if(szFileName[0]==0) FileSaveAsProc(hwndClient);
                                else FileSaveProc(hwndClient);
                                //Make sure it has a filename and which means save was
                                //successful, and then quit.
                                if(szFileName[0]!=0) goto ENDPROGRAM;
                                }
                        }
                else
                        {
ENDPROGRAM:


                        break;
                        }
                 
                //Cancel shutdown
CANCELSHUTDOWN: WinCancelShutdown(hmq, FALSE);
                }

        //Save ini information
        iRes=SaveProfile();

        //Destoy help
        iRes=DestroyHelp();

        //Destroy window.
        WinDestroyWindow(hwndFrame);

        //Destroy message queue
        WinDestroyMsgQueue(hmq);

        //Terminate PM program.
        WinTerminate(hab);
        
        //For printing
        DosFreeMem(pMem);

        return 0;
}
//End Main

//////////////////////////////////////////////////////////////////////////
//ClientWndProc
//
//Handles all messages dispatched to client window.

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

        HPS hps;
        static RECTL rcl;
        int iRes;
        HWND hwndFindDialog;
        HWND hwndReplaceDialog;
        ULONG ulClipboardFormatInfo;
        char szLocalFilename[CCHMAXPATH];
        MRESULT mresult;
        BOOL bTextChanged;

        switch(msg)
        {

                case WM_START_PRINT:
                    //if (SHORT1FROMMP(mp2) == IDM_PRINT)
                        process_print((HWND)mp1);
                    //else
                    //    DosCreateThread(&tid,(PFNTHREAD)process_print,(ULONG)mp1,
                     //  0,0x2000);
                    break;

                case WM_CONTROL:

                 //WinMessageBox(HWND_DESKTOP,hwnd,"WM_CONRTOL","",0,MB_OK);
                 switch (HIUSHORT(mp1))
                        {
                        ///////////////////////////////////////////////////////////////
                        //Process control message sent from MLE control
 
                        
                        //Handle text change
                        case MLN_CHANGE:
 
                                //Change window title?
                                if(bUpdateTitle && iReadFileFinished)
                                {
                                //Add '*' to frame-title to reflect text is modified
                                if(szFileName[0]==0)
                                sprintf(szTitleBuffer,"%s - [untitled]*",szAppName);
                                else
                                sprintf(szTitleBuffer,"%s - [%s]*",
                                        szAppName,szFileName);
                                WinSetWindowText(hwndFrame,szTitleBuffer);
                                bUpdateTitle=FALSE;
                                
                                //Change status bar to MODIFIED
                                bModified=TRUE;
                                UpdateKeyStatus();
                                }

                                //See if undo is still available and set it
                                //appropriately.
                                WinEnableMenuItem(
                                        hwndMenu,
                                        IDM_UNDO,
                                        (WinSendMsg(hwndMLE,
                                                    MLM_QUERYUNDO,
                                                    MPVOID,
                                                    MPVOID)));
              
                                

                        return 0; //MLN_CHANGE

                        //Handle undo buffer overflow.
                        case MLN_UNDOOVERFLOW:
                                //Disable Undo menu
                                WinEnableMenuItem(hwndMenu, IDM_UNDO, FALSE);
                        return 0;

                        //Handle clipboard overflow.
                        case MLN_CLPBDFAIL:
                                WinMessageBox(HWND_DESKTOP,hwnd,
                                                "Clipboard operation failed.",
                                                szAppName,
                                                0,
                                                MB_OK|MB_MOVEABLE|
                                                MB_ICONEXCLAMATION);
                        return 0;

                        //Handle memory errors
                        case MLN_MEMERROR:
                                WinMessageBox(HWND_DESKTOP,hwnd,
                                    "Required memory storage cannot be obtained.",
                                                szAppName,
                                                0,
                                                MB_OK|MB_MOVEABLE|
                                                MB_ICONEXCLAMATION);
 
                        return 0;
 
                        }
 
                return 0;
                ////////////////////////////////////////////////////////////////
 
                case WM_ACTIVATE:
                        UpdateKeyStatus();
                break;

                case WM_SIZE:
                        if(bStatusBarOn)
                        {
                        //Resize MLE window
                        WinSetWindowPos(
                                hwndMLE,
                                HWND_BOTTOM,
                                0,
                                STATUSWND_HEIGHT,
                                SHORT1FROMMP(mp2),
                                (SHORT2FROMMP(mp2)-STATUSWND_HEIGHT),
                                SWP_SIZE);

                         //Resize Status window
                         WinSetWindowPos(
                                 hwndStatus,
                                 HWND_BOTTOM,
                                 0,
                                 0,
                                 SHORT1FROMMP(mp2),
                                 STATUSWND_HEIGHT,
                                 SWP_SIZE);
                        }
                        else //if status bar is off
                        {
                        //Resize MLE window
                        WinSetWindowPos(
                                hwndMLE,
                                HWND_TOP,
                                0,
                                0,
                                SHORT1FROMMP(mp2),
                                SHORT2FROMMP(mp2),
                                SWP_SIZE);

                         //Resize Status window
                         WinSetWindowPos(
                                 hwndStatus,
                                 HWND_BOTTOM,
                                 0,
                                 0,
                                 SHORT1FROMMP(mp2),
                                 STATUSWND_HEIGHT,
                                 SWP_SIZE);

                        }

                return 0; //WM_SIZE

                case WM_CREATE:
                        //Get menu handle
                        hwndMenu=WinWindowFromID(
                                        WinQueryWindow(hwnd, QW_PARENT),
                                        FID_MENU);

                        iRes=UpdateRecentFileMenu();

                        //For printing
                        hps = init_app(hwnd);
 



                return 0; //WM_CREATE

                case WM_PAINT:
                        //Get background color of MLE window
                        INIBackColor=LONGFROMMR(WinSendMsg(hwndMLE,
                                                MLM_QUERYBACKCOLOR,
                                                MPVOID,
                                                MPVOID));

                        hps=WinBeginPaint(hwnd,NULLHANDLE, NULL);
                        WinQueryWindowRect(hwnd, &rcl);
                        //Fill window with default color
                        WinFillRect(hps, &rcl, INIBackColor);
                        WinEndPaint(hps);
                return 0; //WM_PAINT

                case WM_INITMENU:

                    if(mp1==IDM_FILE)
                        {
                        ////////Validate File menu
                       
                        ///////////////////////////////////
                        //See if  Save, Save as, Save and exit are valid
                        //based on whether or not text has changed.
                        bTextChanged=LONGFROMMR(WinSendMsg(hwndMLE,
                                MLM_QUERYCHANGED,
                                MPVOID,
                                MPVOID));
                        if(bTextChanged)
                            {
                            //Enable Save  and Save and exit
                            WinEnableMenuItem(hwndMenu,IDM_SAVE,TRUE);
                            WinEnableMenuItem(hwndMenu,IDM_SAVEANDEXIT,TRUE);
                            }
                        else
                            {
                            //Disable Save and Save and exit
                            WinEnableMenuItem(hwndMenu,IDM_SAVE,FALSE);
                            WinEnableMenuItem(hwndMenu,IDM_SAVEANDEXIT,FALSE); 
                            }

                        ///////////////////////////////////
                        //See if  New and Print are valid
                        //based on text length
                        mresult=WinSendMsg(hwndMLE, MLM_QUERYTEXTLENGTH,
                                                MPVOID,MPVOID);
                        if (mresult==0)
                            {
                            //Disable find, findnext, and replace
                            WinEnableMenuItem(hwndMenu,IDM_NEW,FALSE);
                            WinEnableMenuItem(hwndMenu,IDM_PRINT,FALSE); 
                            }
                        else
                            {
                            //Enable find, findnext, and replace.
                            WinEnableMenuItem(hwndMenu,IDM_NEW,TRUE);
                            WinEnableMenuItem(hwndMenu,IDM_PRINT,TRUE);
                            }
                            ////////////////////////////////////


                        }

                    if(mp1==IDM_EDIT)
                        {
                        ////////Validate EDIT menu.

                        //////////////////////////////
                        //See if Paste is valid. If so
                        //enable the paste menu, if not disable it.
                        if (WinQueryClipbrdFmtInfo(
                                hab,    //Anchor block handle,
                                CF_TEXT,//Format to query
                                &ulClipboardFormatInfo
                                ))
                                WinEnableMenuItem(hwndMenu, IDM_PASTE, TRUE);
                        else    WinEnableMenuItem(hwndMenu, IDM_PASTE, FALSE);
                        ////////////////////////////////

                        //////////////////////////////
                        //See if Cut, Copy and Delete are valid
                        mresult=WinSendMsg(hwndMLE,MLM_QUERYSEL,
                                                    MPFROMSHORT(MLFQS_MINMAXSEL),
                                                    MPVOID);
                        if((SHORT2FROMMR(mresult)-SHORT1FROMMR(mresult))==0)
                            {
                            //Disable cut, copy, delete
                            WinEnableMenuItem(hwndMenu,IDM_CUT,FALSE);
                            WinEnableMenuItem(hwndMenu,IDM_COPY,FALSE);
                            WinEnableMenuItem(hwndMenu,IDM_DELETE,FALSE);
                            }
                        else
                            {
                            //Enable cut,copy, delete
                            WinEnableMenuItem(hwndMenu,IDM_CUT,TRUE);
                            WinEnableMenuItem(hwndMenu,IDM_COPY,TRUE);
                            WinEnableMenuItem(hwndMenu,IDM_DELETE,TRUE);
                            }
                        /////////////////////////////////

                        ////////////////////////////////
                        //See if find, findnext, and replace are valid.
                        mresult=WinSendMsg(hwndMLE, MLM_QUERYTEXTLENGTH,
                                                MPVOID,MPVOID);
                        if (mresult==0)
                            {
                            //Disable find, findnext, and replace
                            WinEnableMenuItem(hwndMenu,IDM_FIND,FALSE);
                            WinEnableMenuItem(hwndMenu,IDM_FINDNEXT,FALSE);
                            WinEnableMenuItem(hwndMenu,IDM_REPLACE,FALSE);
                            }
                        else
                            {
                            //Enable find, findnext, and replace.
                            WinEnableMenuItem(hwndMenu,IDM_FIND,TRUE);
                            WinEnableMenuItem(hwndMenu,IDM_FINDNEXT,TRUE);
                            WinEnableMenuItem(hwndMenu,IDM_REPLACE,TRUE);
                            }
                        ///////////////////////////////////

                        ///////////////////////////////////
                        //See if undo is possible
                        mresult=WinSendMsg(hwndMLE, MLM_QUERYUNDO,
                                                MPVOID, MPVOID);
                        if(mresult==0)
                            //Disable undo menu
                            WinEnableMenuItem(hwndMenu,IDM_UNDO,FALSE);
                        else
                            //Enable undo menu
                            WinEnableMenuItem(hwndMenu,IDM_UNDO,TRUE);
                        ////////////////////////////////////
                               

                    
                        }        
                                
                return 0;//WM_INITMENU                              

                case WM_CHAR:
 
                //Don't process key-up messages
                if (CHARMSG(&msg)->fs & KC_KEYUP) return 0;
                //Don't process control
                if (CHARMSG(&msg)->fs & KC_CTRL) return 0;
                //Don't process alt
                if (CHARMSG(&msg)->fs & KC_ALT) return 0;
                
 
                switch(CHARMSG(&msg)->vkey)
                        {
 

                        //Handle Tab characters
                        case  VK_TAB:
                            TabProc();
                        break;  //VK_TAB
                        }
                return 0;//WM_CHAR

                case WM_COMMAND:
                        //Swith for menu commands
                        switch(COMMANDMSG(&msg) ->cmd)
                        {
                                case IDM_NEW:
                                        FileNewProc(hwnd);

                                 return 0; //IDM_NEW

                                 case IDM_OPEN:
                                       iRes=FileOpenProc(hwnd,"");

                                 return 0; //IDM_OPEN

                                 case IDM_SAVE:
                                    if (szFileName[0]==0)
                                       {
                                       FileSaveAsProc(hwnd);

                                       }
                                    else
                                       {
                                       FileSaveProc(hwnd);

                                       }

                                 return 0; //IDM_SAVE

                                 case IDM_SAVEAS:
                                        FileSaveAsProc(hwnd);

                                 return 0;

                                 case IDM_SAVEANDEXIT:
                                     //Save the file
                                     if (szFileName[0]==0)
                                       {
                                       FileSaveAsProc(hwnd);

                                        //User may have cancelled save as.
                                        //if so, don't exit program.
                                       if(szFileName[0]==0) return 0;
                                       }
                                     else
                                       {
                                       FileSaveProc(hwnd);
                                       }
                                    WinSendMsg(hwnd,WM_CLOSE, 0L, 0L);
                                 return 0;
 
                                 case IDM_EXIT:
                                    WinSendMsg(hwnd,WM_CLOSE, 0L, 0L);
                                 return 0; //IDM_EXIT

                                 case IDM_ABOUT:
                                     WinDlgBox(HWND_DESKTOP, hwnd, AboutDlgProc,
                                                NULLHANDLE, IDD_ABOUT2, NULL);

                                 return 0; //IDM_ABOUT


                                 case IDM_UNDO:
                                        WinSendMsg(hwndMLE,
                                        MLM_UNDO,
                                        NULL,
                                        NULL);
                                 return 0;

                                 case IDM_CUT:
                                        WinSendMsg(hwndMLE,
                                        MLM_CUT,
                                        NULL,
                                        NULL);
                                 return 0;

                                 case IDM_COPY:
                                        WinSendMsg(hwndMLE,
                                        MLM_COPY,
                                        NULL,
                                        NULL);
                                 return 0;

                                 case IDM_PASTE:
                                        WinSendMsg(hwndMLE,
                                        MLM_PASTE,
                                        NULL,
                                        NULL);
                                 return 0;

                                 case IDM_DELETE:
                                        WinSendMsg(hwndMLE,
                                        MLM_CLEAR,
                                        NULL,
                                        NULL);
                                 return 0;

                                 case IDM_FIND:
                               
                                        hwndFindDialog=WinLoadDlg(HWND_DESKTOP,
                                                hwnd,
                                                FindDlgProc,
                                                NULLHANDLE,
                                                IDD_FIND,
                                                NULL);

                                 return 0; //IDM_FIND
 
                                 case IDM_FINDNEXT:
                                        FindNextProc();
                                 return 0;//IDM_FINDNEXT

                                 case IDM_REPLACE:
                                        hwndReplaceDialog=WinLoadDlg(HWND_DESKTOP,
                                                hwnd,
                                                ReplaceDlgProc,
                                                NULLHANDLE,
                                                IDD_REPLACE,
                                                NULL);
                                 return 0;
 
                                 case IDM_TABSETTINGS:
                                        WinLoadDlg(HWND_DESKTOP,
                                                hwnd,
                                                TabSetDlgProc,
                                                NULLHANDLE,
                                                IDD_TABSETTINGS,
                                                NULL);
                                 return 0; //IDM_TABSETTINGS

                                 case IDM_SETWRAP:
                                        //Enable or disable word-wrap
                                        WinSendMsg(hwndMLE,
                                                MLM_SETWRAP,
                                                MPFROMSHORT(
                                                  !( WinIsMenuItemChecked(
                                                      hwndMenu,IDM_SETWRAP))
                                                ),
                                                MPVOID);

                                        //Check or uncheck wrap menu item.
                                        WinCheckMenuItem(hwndMenu,IDM_SETWRAP,
                                          !(WinIsMenuItemChecked(
                                                hwndMenu,IDM_SETWRAP)));
 
                                 return 0; //IDM_SETWRAP

                                case IDM_STATUSBAR:

                                       //Check or uncheck menu item.
                                        WinCheckMenuItem(hwndMenu,IDM_STATUSBAR,
                                          !(WinIsMenuItemChecked(
                                                hwndMenu,IDM_STATUSBAR)));

                                        //Enable or disable status bar
                                        if(WinIsMenuItemChecked(
                                                hwndMenu,IDM_STATUSBAR))
                                           TurnStatusBarOn();
                                        else
                                           TurnStatusBarOff();

                                return 0;//IDM_STATUSBAR
 
                                case IDM_FONT:
                                        FontPalProc();
                                return 0;

                                case IDM_COLOR:
                                        ColorPalProc();
                                return 0;

                                case IDM_SAVESETTINGS:
                                        if(SaveProfile())
                                        WinMessageBox(HWND_DESKTOP, 
                                                hwndClient,
                                                "Settings saved.",
                                                szAppName,
                                                0,
                                                MB_OK|MB_INFORMATION|
                                                MB_MOVEABLE);
                                        else
                                        WinMessageBox(HWND_DESKTOP,
                                                hwndClient,
                                                "Settings could not be saved",
                                                szAppName,
                                                0,
                                                MB_OK|MB_ICONEXCLAMATION|
                                                MB_MOVEABLE);
                                return 0;

                                //Handle recent open file menu
                                case IDM_RECENTFILE1:
                                        strcpy(szLocalFilename,szRecentFile1);
                                        iRes=FileOpenProc(hwndClient,szLocalFilename);
                                return 0;
                                case IDM_RECENTFILE2:
                                        strcpy(szLocalFilename,szRecentFile2);
                                        iRes=FileOpenProc(hwndClient,szLocalFilename);
                                return 0;
                                case IDM_RECENTFILE3:
                                        strcpy(szLocalFilename,szRecentFile3);
                                        iRes=FileOpenProc(hwndClient,szLocalFilename);
                                return 0;
                                case IDM_RECENTFILE4:
                                        strcpy(szLocalFilename,szRecentFile4);
                                        iRes=FileOpenProc(hwndClient,szLocalFilename);
                                return 0;
                                case IDM_RECENTFILE5:
                                        strcpy(szLocalFilename,szRecentFile5);
                                        iRes=FileOpenProc(hwndClient,szLocalFilename);
                                return 0;
                                case IDM_RECENTFILE6:
                                        strcpy(szLocalFilename,szRecentFile6);
                                        iRes=FileOpenProc(hwndClient,szLocalFilename);
                                return 0;
                                case IDM_RECENTFILE7:
                                        strcpy(szLocalFilename,szRecentFile7);
                                        iRes=FileOpenProc(hwndClient,szLocalFilename);
                                return 0;
                                case IDM_RECENTFILE8:
                                        strcpy(szLocalFilename,szRecentFile8);
                                        iRes=FileOpenProc(hwndClient,szLocalFilename);
                                return 0;
                                case IDM_RECENTFILE9:
                                        strcpy(szLocalFilename,szRecentFile9);
                                        iRes=FileOpenProc(hwndClient,szLocalFilename);
                                return 0;


                                //Handle auto indent menu option
                                case IDM_AUTOINDENT:
                                        bUseAutoIndent=!bUseAutoIndent;
                                        WinCheckMenuItem(hwndMenu,
                                                IDM_AUTOINDENT,
                                                bUseAutoIndent);
                                return 0;

                                //Handle AutoSave menu
                                 case IDM_AUTOSAVE:
                                     WinDlgBox(HWND_DESKTOP, hwnd, AutoSaveDlgProc,
                                                NULLHANDLE, IDD_AUTOSAVE, NULL);

                                 return 0; //IDM_AUTOSAVE

                                case IDM_CASESENSITIVEFILENAMES:
                                  gbCaseSensitiveFileNames=!gbCaseSensitiveFileNames;
                                  WinCheckMenuItem(hwndMenu,
                                                IDM_CASESENSITIVEFILENAMES,
                                                gbCaseSensitiveFileNames);
                                    
                                return 0; //IDM_CASESENSITIVEFILENAMES
                       
                                //Handle File-Spawn ZED
                                case IDM_SPAWNZED:
                                        //Save current settings.
                                        SaveProfile();
                                        iRes=SpawnProc();
                                return 0;                     

                                //Handle help messages
                                case IDM_USINGHELP:
                                     HelpUsingHelp();
                                return 0;
                                case IDM_HELPINDEX:
                                     HelpIndex();
                                return 0;
                                case IDM_HELPCONTENTS:
                                     HelpContents();
                                return 0;
                                case IDM_GENERALHELP:
                                     HelpGeneral();
                                return 0;

                                case IDM_KEYSHELP:
                                     HelpKeys();
                                return 0;

                                //For printing
                                case IDM_CHOOSE_PRINTER:
                                    WinDlgBox(HWND_DESKTOP,
                                                hwndFrame,
                                                (PFNWP)SelectPrinterDlgProc,
                                                0,
                                                SELECT_PRINTER_DLG,
                                                (PVOID)0);
                                return 0;

                                case IDM_PRINTER_INFO:
                                WinDlgBox(HWND_DESKTOP,
                                            hwndFrame,
                                            (PFNWP)PrintInfoDlgProc,
                                            0,
                                            QUEUE_INFO_DLG,
                                            (PVOID)0);
                                return 0;

                                case IDM_PRINT:
                                 /* Confirm the print operation and print */
                                if (WinDlgBox(HWND_DESKTOP,
                                                    hwndClient,
                                                    PrintDlgProc,
                                                    0,
                                                    PRINT_DLG,
                                                    (PVOID)0) == DID_OK)
                                {
                                WinDlgBox(HWND_DESKTOP,
                                                hwndClient,
                                                PrintStatusProc,
                                                0,
                                                PRINT_STATUS_DLG,
                                (PVOID)&mp1);
                                }
                                break;//IDM_PRINT

                                case IDM_IMPORT:
                                    iRes=ImportFileProc();
                                return 0; //IDM_IMPORT

                                case IDM_REGISTRATION:
                                    HelpFirstTime();
                                return 0;


                        } //End Switch for Menu Commands
                      

        }//End Switch for window messages
        
        //Return default window procedure for message.
        return WinDefWindowProc(hwnd, msg, mp1,mp2);
}
// End ClientWndProc 

//////////////////////////////////////////////////////////////////////////////
//EnableFileMenu
//Enables or disables the file operations menus.
//
//Created       :       01/15/1994      Bruce Shankle
//
//When a read thread is processing, the file menus except Exit should be 
//disable. 
//
void EnableFileMenu(BOOL enable)
{
  WinEnableMenuItem(hwndMenu, IDM_NEW, enable);
  WinEnableMenuItem(hwndMenu, IDM_OPEN, enable);
  WinEnableMenuItem(hwndMenu, IDM_SAVE, enable);
  WinEnableMenuItem(hwndMenu, IDM_SAVEAS, enable);
  WinEnableMenuItem(hwndMenu, IDM_SAVEANDEXIT, enable);
  WinEnableMenuItem(hwndMenu, IDM_EDIT, enable);
  WinEnableMenuItem(hwndMenu, IDM_RECENTFILE1, enable);
  WinEnableMenuItem(hwndMenu, IDM_RECENTFILE2, enable);
  WinEnableMenuItem(hwndMenu, IDM_RECENTFILE3, enable);
  WinEnableMenuItem(hwndMenu, IDM_RECENTFILE4, enable);
  WinEnableMenuItem(hwndMenu, IDM_RECENTFILE5, enable);
  WinEnableMenuItem(hwndMenu, IDM_RECENTFILE6, enable);
  WinEnableMenuItem(hwndMenu, IDM_RECENTFILE7, enable);
  WinEnableMenuItem(hwndMenu, IDM_RECENTFILE8, enable);
  WinEnableMenuItem(hwndMenu, IDM_RECENTFILE9, enable);
}
//End EnableFileMenu


