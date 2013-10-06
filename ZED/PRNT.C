///////////////////////////////////////////////////////////////////
//PRNT.C
//This file contains the functions and
//window procedures to implement printing.
//
//Created:    02/26/1995     Bruce Shankle

#define INCL_WIN
#define INCL_WINERRORS
#define INCL_GPI
#define INCL_DEV
#define INCL_DEVERRORS
#define INCL_ERRORS
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_DOSPROCESS

#define INCL_WINSTDFILE
#define INCL_WINSTDFONT
#define INCL_WINHELP
#define INCL_WINENTRYFIELDS
#define INCL_WINLISTBOXES
#define INCL_WINDIALOGS
#define INCL_WINERRORS
#define INCL_WINWINDOWMGR
#define INCL_WINSHELLDATA
#define INCL_GPIPRIMITIVES
#define INCL_GPITRANSFORMS
#define INCL_GPIBITMAPS
#define INCL_GPICONTROL
#define INCL_GPIMETAFILES
#define INCL_GPILCIDS


#include <os2.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "prnt.h"
#include "dialog.h"
#include "zed.h"


#define NERR_BuffTooSmall 2123

//For printing with current fon
#define LCID_FONT 1


extern PVOID  pMem;
HWND   hHintWnd;
CHAR   szTitle[64];
CHAR   szBuff1[CCHMAXPATH];
CHAR   szFontFace[] = "8.Helv";
CHAR   szPM_Q_STD[] = "PM_Q_STD";

int    nSysFontHeight;
LONG   lColorWorkSpace;
USHORT usDraw = IDM_DISP_BLANK;
CHAR   szPriority[3][10] =
{ 
  "Low",
  "Standard",
  "High"
};

LONG lPriority[3] =
{
   1,50,99
};

/* Array of pointers to priority strings (for spinner control) */
PVOID pszPriorities[3] = 
{
    &szPriority[0],
    &szPriority[1],
    &szPriority[2]
};


DEVICEINFO CurrentDI;


/////////////////////////////////////////////////////////////
//draw_pages
//This function draws the text on
//the passed pDI->hPrintPS and inserts
//page breaks as neccesary.

int  APIENTRY draw_pages(PDEVICEINFO pDI)
{

POINTL point;    //Point for GpiMoves
SIZEL sizel;        //Size of presentation space.
PCHAR pLineBuffer;        //Pointer to line buffer.
ULONG LINE_BUFFER_SIZE=1000;
ULONG ulNumLines;
IPT lLineLength;
IPT lLineStart;
ULONG ulBytesExported;
long lLoopCtr;
ULONG ulLoopCtr;
long lRes;
PFONTMETRICS pfm;    //Pointer to font metrics.

//Next variable for font changes
//FATTRS fat; //Font attributes



pfm=(PFONTMETRICS) calloc(1, sizeof ( FONTMETRICS ) );
if (pfm==NULL)
    {
    sprintf(buffer,"Could not allocate memory for font metrics.");
    WinMessageBox(HWND_DESKTOP, hwndClient,buffer,szAppName,0,NULL);
    return 0;
    }

//Allocate 1000 bytes of memory for line buffer.
pLineBuffer= (PCHAR) calloc(1, LINE_BUFFER_SIZE);
if (pLineBuffer==NULL)
    {
    sprintf(buffer,"Could not allocate memory for line buffer.");
    WinMessageBox(HWND_DESKTOP, hwndClient,buffer,szAppName,0,NULL);
    free(pfm);
    return 0;
    }

///////////////////////////////////////
//Code below used for using window font
//commented out for shareware release version 1.0
//Allocate memory for font metrics.
/*
//Get MLE font attributes
WinSendMsg(hwndMLE,
        MLM_QUERYFONT,
        MPFROMP( (PFATTRS) &fat),
        MPVOID);

//Set font of printer presentation space to that of MLE
GpiCreateLogFont(pDI->hPrintPS, NULL, LCID_FONT, &fat);
GpiSetCharSet(pDI->hPrintPS, LCID_FONT);
*/
//Uncomment out above code to enable using different font
///////////////////////////////////////////////////



//Set MLE export buffer to point to pLineBuffer
WinSendMsg(hwndMLE,
        MLM_SETIMPORTEXPORT,
        MPFROMP(pLineBuffer),
        MPFROMLONG( LINE_BUFFER_SIZE));

//Get number of lines
ulNumLines = LONGFROMMR ( WinSendMsg (
                                                hwndMLE,
                                                MLM_QUERYLINECOUNT,
                                                MPVOID,MPVOID));


//Get font metrics.
GpiQueryFontMetrics(pDI->hPrintPS, sizeof(FONTMETRICS), pfm);

//Get size of presentation space
lRes=GpiQueryPS ( pDI->hPrintPS, (PSIZEL) &sizel);

//sprintf(buffer,"Width: %d, Height: %d",sizel.cx, sizel.cy);
//WinMessageBox(HWND_DESKTOP, hwndClient,buffer,"",0,NULL);

//Set up initial point at top of page. 
point.x=0;
point.y=sizel.cy;

//Print all lines
for (lLoopCtr=0; lLoopCtr<ulNumLines; lLoopCtr++)
    {
    //Move down to below highest character
    point.y-=pfm->lMaxAscender;
    GpiMove(pDI->hPrintPS, &point);

    //Get start of current line
    lLineStart=LONGFROMMR ( WinSendMsg( hwndMLE,
                                                                    MLM_CHARFROMLINE,
                                                                    MPFROMLONG(lLoopCtr),
                                                                    MPVOID));

    //Get length or current line
    lLineLength=LONGFROMMR( WinSendMsg ( hwndMLE,
                                                                    MLM_QUERYLINELENGTH,
                                                                    MPFROMLONG(lLineStart),
                                                                    MPVOID));

    //Empty the buffer
    memset(pLineBuffer, 0, LINE_BUFFER_SIZE); 

    //Export line to line buffer
    ulBytesExported=LONGFROMMR( WinSendMsg (hwndMLE,
                                                                    MLM_EXPORT,
                                                                    MPFROMP( (PIPT) &lLineStart),
                                                                    MPFROMP( (PULONG) &lLineLength)));
                                                  
    //Remove carriage return and line-feed characters
    for (ulLoopCtr=0; ulLoopCtr<LINE_BUFFER_SIZE; ulLoopCtr++)
        {
        if(pLineBuffer[ulLoopCtr]==0x0D) pLineBuffer[ulLoopCtr]=0;
        if(pLineBuffer[ulLoopCtr]==0x0A) pLineBuffer[ulLoopCtr]=0;                          
        }

    //Draw the line on the printer presentation space
    GpiCharString(pDI->hPrintPS,strlen(pLineBuffer), pLineBuffer);    
    
    //Move down to next base
    point.y-=pfm->lMaxDescender;

    //Temporary triple spacing.
//    point.y-=pfm->lMaxAscender;
//    point.y-=pfm->lMaxDescender;
//    point.y-=pfm->lMaxAscender;
//    point.y-=pfm->lMaxDescender;


    //Is is time for a page break?
    if (point.y<=( (pfm->lMaxDescender)+(pfm->lMaxAscender) ))
        {

        //Do page break.
        lRes=DevEscape(pDI->hPrintDC,DEVESC_NEWFRAME, 0L, 
                                    (PBYTE) NULL,
                                    (PLONG) NULL,
                                    (PBYTE) NULL);

        //Check for devescape error
        if (lRes!=DEV_OK)
            {
            WinMessageBox(HWND_DESKTOP, hwndClient,
                "DevEscape error in function draw_pages while printing",
                            szAppName,0,NULL);
            //Free memory
            free(pfm);
            free(pLineBuffer);
            return -1;
            }


        //Set point y back to top
        point.y=sizel.cy;

        }
    
    }

//Free memory
free(pfm);
free(pLineBuffer);

///////////////////////////////////////////
//Commented out for shareware release 1.0
//Reset font and delete logical font
//GpiSetCharSet(pDI->hPrintPS, LCID_DEFAULT);
//GpiDeleteSetId(pDI->hPrintPS, LCID_FONT);


return 0;
}
//End draw_pages
///////////////////////////////////////////////////////////////



VOID APIENTRY center_window (HWND hWnd)
{
    ULONG ulScrWidth, ulScrHeight;
    RECTL Rectl;

    ulScrWidth  = WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN);
    ulScrHeight = WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN);
    WinQueryWindowRect (hWnd, &Rectl);
    WinSetWindowPos (hWnd, HWND_TOP, (ulScrWidth-Rectl.xRight)/2,
        (ulScrHeight-Rectl.yTop)/2, 0, 0, SWP_MOVE | SWP_ACTIVATE);
    return;
}


BOOL APIENTRY create_info_DC(PPRQINFO3 pPrq3,PDEVICEINFO pDI)
/*-----------------------------------*\


   This function will intialize the pDI DEVICEINFO structure based
   on the queue pointed to by pPrq3. If pDI contains an existing 
   entry its' info DC it is destroyed and any form info is freed.
   A new info DC is created and the get_page_metrics function is used
   to initialize size information in the stucture.

\*-----------------------------------------------------------------*/
{
    PDRIVDATA    pDD;
    PPRDINFO3    pPrd3;
    DEVOPENSTRUC DevOpenData;
    SIZEL        sizel;
    ULONG        ulNeeded = 0;
    int          i;
    CHAR         szDriverName[MAX_DEVICENAME];
    CHAR         szDeviceName[MAX_DEVICENAME];

    /* Free existing info DCs and PSs */
    if (pDI->hInfoPS)
    {
        GpiAssociate(pDI->hInfoPS,0);
        GpiDestroyPS(pDI->hInfoPS);
        pDI->hInfoPS = 0;
        DevCloseDC(pDI->hInfoDC);
        pDI->hInfoDC = 0;
    }  
    if (pDI->pHCInfo)
    {
        DosSubFree(pMem,pDI->pHCInfo,(pDI->lSizeHC * sizeof(HCINFO)));
        pDI->pHCInfo = 0;
        pDI->lSizeHC = 0;
    }

    if ((i = strcspn(pPrq3->pszDriverName,".")) != 0)
    {
        strncpy((PSZ)szDriverName,(PSZ)pPrq3->pszDriverName,i);
        szDriverName[i] = '\0';
        strcpy((PSZ)szDeviceName,(PSZ)&pPrq3->pszDriverName[i + 1]);
    }

    if (SplQueryDevice(0,pPrq3->pszPrinters,3,0,0,&ulNeeded) != NERR_BuffTooSmall)
        return(FALSE);

    if (DosSubAlloc(pMem,(PPVOID)&pPrd3,ulNeeded))
        return (FALSE);
    if (SplQueryDevice(0,pPrq3->pszPrinters,3,(PVOID)pPrd3,ulNeeded,
            &ulNeeded) == 0)
    {
        if (pDI->pDrivData)
            pDD = pDI->pDrivData;
        else
            pDD = pPrq3->pDriverData;

        strcpy(pDI->szQueueDesc,pPrq3->pszComment);
        strcpy(pDI->szDriverName,pPrq3->pszDriverName);
        strcpy(pDI->szQueueName,pPrq3->pszName);

        DevOpenData.pszLogAddress = pPrd3->pszLogAddr;
        DevOpenData.pszDriverName = szDriverName;
        DevOpenData.pdriv         = pDD;
        DevOpenData.pszDataType   = (PSZ)szPM_Q_STD;


        if ((pDI->hInfoDC = DevOpenDC (hab,OD_INFO,"*",4,
                                (PVOID)&DevOpenData,0)) != 0)
        {
            sizel.cx = 0;
            sizel.cy = 0;
            if(!(pDI->hInfoPS = GpiCreatePS (hab,pDI->hInfoDC,&sizel,GPIA_ASSOC | GPIT_MICRO | PU_LOMETRIC)))
            {
                DevCloseDC(pDI->hInfoDC);
                pDI->hInfoDC = 0;
            }  
            else
            {
                get_page_metrics(pDI);
            }
        }
    }
    else
       post_error(hwndClient,ID_ERR_FAIL_DEV_QUERY);

    DosSubFree(pMem,pPrd3,ulNeeded);
    return((BOOL)pDI->hInfoPS);
}


BOOL APIENTRY create_print_DC(PDEVICEINFO pDI)
/*-----------------------------------------------------------------*\

   This function will create an OD_QUEUED device context based on the
   contents of the pDI parameter. If successful a presentation space
   is created and the DEVESC_STARTDOC escape is issued. The presentation
   space is then reset from indexed to RGB mode.

\*-----------------------------------------------------------------*/
{
    DEVOPENSTRUC  DevOpenStr;
    CHAR          szQueueProcParams[9];
    CHAR          szSpoolerParams[9];
    CHAR          szDriverName[MAX_DEVICENAME];
    int           i;

    memset((PVOID)&DevOpenStr, 0, sizeof(DevOpenStr));

    /* Set priority in spooler params */
    sprintf((PSZ)szSpoolerParams,(PSZ)"PRTY=%d",pDI->lPriority);
    /* Set number of copies in the queue processor params */
    sprintf((PSZ)szQueueProcParams,(PSZ)"COP=%d",pDI->lCopies);

    //WinLoadString(hab,0,IDS_JOB_TITLE,sizeof(szBuff1),szBuff1);
    strcpy(szBuff1,szFileName);

    if ((i = strcspn(pDI->szDriverName,".")) != 0)
    {
        strncpy((PSZ)szDriverName,(PSZ)&pDI->szDriverName,i);
        szDriverName[i] = '\0';
    }

    DevOpenStr.pszLogAddress      = pDI->szQueueName;
    DevOpenStr.pszDriverName      = szDriverName;
    DevOpenStr.pdriv              = pDI->pDrivData;
    DevOpenStr.pszDataType        = (PSZ)szPM_Q_STD;
    DevOpenStr.pszComment         = (PSZ)szBuff1;
    DevOpenStr.pszQueueProcParams = (PSZ)szQueueProcParams;
    DevOpenStr.pszSpoolerParams   = (PSZ)szSpoolerParams;

    if ((pDI->hPrintDC = DevOpenDC(hab,OD_QUEUED,"*",9,
                             (PVOID)&DevOpenStr,(HDC)0)) != 0)
    {
        if (!(pDI->hPrintPS = GpiCreatePS(hab,pDI->hPrintDC,(PSIZEL)&pDI->PageSizel,
                                         PU_LOMETRIC | GPIA_ASSOC )))
        {
            DevCloseDC(pDI->hPrintDC);
            pDI->hPrintDC = 0;
        }  
        else
        {
            DevEscape(pDI->hPrintDC,DEVESC_STARTDOC,strlen(szBuff1),szBuff1,0,0);
            GpiCreateLogColorTable (CurrentDI.hPrintPS,LCOL_RESET,LCOLF_RGB,0,0,NULL);
        }
    }
    else
        WinGetLastError(hab);
    return((BOOL)pDI->hPrintPS);
}


void APIENTRY create_status_line(HWND hwndClient)
/*-----------------------------------*\

   This function creates a static window positioned across the bottom
   of the client window which will display hint text when menus are 
   browsed.

\*-----------------------------------------------------------------*/
{
    FONTMETRICS fontMetrics;
    HPS    hPS  = WinGetPS(hwndClient);
    RECTL  Rectl;
    LONG   lColor;

    WinQueryWindowRect(hwndClient,(PRECTL)&Rectl);
    GpiQueryFontMetrics(hPS,sizeof(FONTMETRICS),(PFONTMETRICS)&fontMetrics);
    nSysFontHeight = fontMetrics.lMaxBaselineExt + 4;
    WinSendMsg(hwndClient,WM_SIZE,0,MPFROM2SHORT(Rectl.xRight,Rectl.yTop));

    hHintWnd = WinCreateWindow (hwndClient,WC_STATIC,"",
            WS_VISIBLE | SS_TEXT | DT_LEFT | DT_VCENTER,
            0,0,Rectl.xRight,nSysFontHeight,
            hwndClient,HWND_TOP,1,NULL,NULL);
    lColor = CLR_PALEGRAY;
    WinSetPresParam(hHintWnd,PP_BACKGROUNDCOLORINDEX,sizeof(PLONG),&lColor);
    lColor = CLR_BLACK;
    WinSetPresParam(hHintWnd,PP_FOREGROUNDCOLORINDEX,sizeof(PLONG),&lColor);
    WinSetPresParam(hHintWnd,PP_FONTNAMESIZE,strlen(szFontFace)+1,(PVOID)szFontFace);

    WinReleasePS(hPS);
}


void APIENTRY display_hint(SHORT sMenuID)
/*-----------------------------------*\

   This function will load a menu hint string based on the supplied
   id and display that string in the status window.

\*-----------------------------------------------------------------*/
{
    CHAR  szBuff2[MAX_BUFF];

    if (sMenuID == -1)
        szBuff2[0] = '\0';
    else
    {
        WinLoadString(hab,0,sMenuID,sizeof(szBuff2),szBuff2);
        if ( (sMenuID == IDM_PRINT) || (sMenuID == IDM_PRINT_THREAD) )
            strcat(szBuff2,CurrentDI.szQueueDesc);
    }
    WinSetWindowText(hHintWnd,szBuff2);
}



PPRQINFO3 APIENTRY enum_print_queues(HWND hWnd,HWND hWndLBox,PULONG pulSize,
                                     PULONG pulCount,PULONG pulCurrent)
/*-----------------------------------*\

   This function will enumerate the installed print queues and add
   their description to hWndLBox.  The function returns a pointer
   to an array of PPRQINFO3 structures if successful. The number of
   entries in the array and the total size of the allocated memory 
   is returned in the pulCount and pulSize parameters respectfully.
   The index of the current application default queue is returned in
   the pulCurrent parameter.

\*-----------------------------------------------------------------*/
{
    ULONG     cReturned, cTotal, ulSize;
    int       i;
    PSZ       pName;
    PPRQINFO3 pQueueInfo = 0;

    ulSize = 0;
    SplEnumQueue(0,3,0,0L,(PULONG)&cReturned,(PULONG)&cTotal,
        (PULONG)&ulSize,0);    

    if (pulCount)
        *pulCount = cTotal;

    if (!cTotal)
       post_error(hWnd,ID_NOQUEUE_ERROR);
    else if (!DosSubAlloc(pMem,(PPVOID)&pQueueInfo,ulSize))
    {
        /* Set the size in the return parameter */
        *pulSize = ulSize;

        SplEnumQueue(0,3,pQueueInfo,ulSize,(PULONG)&cReturned,(PULONG)&cTotal,
            (PULONG)&ulSize,0);    

        WinEnableWindowUpdate(hWndLBox,FALSE);
        WinSendMsg (hWndLBox,LM_DELETEALL,0,0);
        for (i = 0;i < (int)cTotal; i++)
        {
            /* If the queue description is a NULL string then use the
               the queue name so the dialog won't have a blank entry */
            if (*pQueueInfo[i].pszComment)
                pName = pQueueInfo[i].pszComment;
            else
                pName = pQueueInfo[i].pszName;
            WinSendMsg (hWndLBox,LM_INSERTITEM,(MPARAM)LIT_END,(MPARAM)pName);
            if (strcmp(CurrentDI.szQueueDesc,pName) == 0)
                /* This is the current application default */
                *pulCurrent = i;
        }
        WinEnableWindowUpdate(hWndLBox,TRUE);
    }
    return(pQueueInfo);
}




VOID APIENTRY get_job_properties(PPRQINFO3 pQueueInfo,PDDINFO pDDInfo,
                                 ULONG ulOption)
/*-----------------------------------*\

   This function will query or post job properties for the queue
   identified by the pQueueInfo parameter. ulOptioh should be set
   to DPDM_QUERYJOBPROP or DPDM_POSTJOBPROP.
   
\*-----------------------------------------------------------------*/
{
   int   i;
   PSZ   pszTemp;
   CHAR  szDriverName[MAX_DEVICENAME];
   CHAR  szDeviceName[MAX_DEVICENAME];

   if ((i = strcspn(pQueueInfo->pszDriverName,".")) != 0)
   {
      strncpy((PSZ)szDriverName,(PSZ)pQueueInfo->pszDriverName,i);
      szDriverName[i] = '\0';
      strcpy((PSZ)szDeviceName,(PSZ)&pQueueInfo->pszDriverName[i + 1]);
   }
   else
   {
      strcpy((PSZ)szDriverName, pQueueInfo->pszDriverName);
      *szDeviceName = '\0';
   }

   pszTemp = (PSZ)strchr(pQueueInfo->pszPrinters, ',');
   if ( pszTemp )
   {
      *pszTemp = '\0' ;
   }

   if (pDDInfo->pDrivData == 0)
   {
       /* Allocate memory for the driver data */
       pDDInfo->lSizeDD = DevPostDeviceModes(hab,0,szDriverName,szDeviceName,
           pQueueInfo->pszPrinters,ulOption);
       DosSubAlloc(pMem,(PPVOID)&pDDInfo->pDrivData,pDDInfo->lSizeDD);
       pDDInfo->pDrivData->cb = sizeof(DRIVDATA);
       pDDInfo->pDrivData->lVersion = 0;
       strcpy(pDDInfo->pDrivData->szDeviceName,szDeviceName);
   }
   DevPostDeviceModes(hab,pDDInfo->pDrivData,szDriverName,szDeviceName,
       pQueueInfo->pszPrinters,ulOption);
}  



VOID APIENTRY get_page_metrics(PDEVICEINFO pDI)
/*-----------------------------------*\

   This function will initialize the physical and printable page
   sizes for the information DC in the pDI parameter.

\*-----------------------------------------------------------------*/
{
    POINTL   PtlRes;
    PHCINFO  pHCInfo;
    int      i;

    GpiQueryPS(pDI->hInfoPS,(PSIZEL)&pDI->PageSizel);
    pDI->lSizeHC = DevQueryHardcopyCaps(pDI->hInfoDC,0,0,0);
    if ( (pDI->lSizeHC > 0) &&
         (DosSubAlloc(pMem,(PPVOID)&pDI->pHCInfo,pDI->lSizeHC * sizeof(HCINFO)) == 0) )
    { 
        DevQueryHardcopyCaps(pDI->hInfoDC,0,pDI->lSizeHC,pDI->pHCInfo);
        pHCInfo = pDI->pHCInfo;
        for (i = 0; i < pDI->lSizeHC; i++)
        {
            if (pHCInfo[i].flAttributes & HCAPS_CURRENT)
            {
                /* Calculate the physical page size */
                DevQueryCaps (pDI->hInfoDC,CAPS_HORIZONTAL_RESOLUTION,1L,
                     (PLONG) &PtlRes.x);
                DevQueryCaps (pDI->hInfoDC,CAPS_VERTICAL_RESOLUTION,1L,
                    (PLONG) &PtlRes.y);
                /* Convert from millimeters to pixels */
                pDI->PhyPageSizel.cx = (PtlRes.x * pHCInfo[i].cx) / 1000;
                pDI->PhyPageSizel.cy = (PtlRes.y * pHCInfo[i].cy) / 1000;
                /* Convert from device to world coordinate space */
                GpiConvert(pDI->hInfoPS,CVTC_DEVICE,CVTC_WORLD,1,(PPOINTL)&pDI->PhyPageSizel);

                pDI->PrintOffsetSizel.cx = (PtlRes.x * pHCInfo[i].xLeftClip) / 1000;
                pDI->PrintOffsetSizel.cy = (PtlRes.y * pHCInfo[i].yBottomClip) / 1000;
                /* Convert from device to world coordinate space */
                GpiConvert(pDI->hInfoPS,CVTC_DEVICE,CVTC_WORLD,1,(PPOINTL)&pDI->PrintOffsetSizel);
                break;
            }
        }
    }
    else
    {
        pDI->pHCInfo = 0;    
        pDI->lSizeHC = 0;
    }
}


HPS APIENTRY init_app(HWND hWnd)
/*-----------------------------------*\

   This function is called on program startup to intialize the CurrentDI
   variable based on the system default queue. A PS is created for the
   client window, the default viewing matrix is modified, and the PS
   is changed from indexed to RGB mode.

\*-----------------------------------------------------------------*/
{
    PPRQINFO3 pPrq3;
    ULONG     ulSize;
    char      szTemp[MAX_BUFF];
    PSZ       pStr;
    ULONG     ulNeeded;
    MATRIXLF  matlfDefView; 

    memset(&CurrentDI,0,sizeof(DEVICEINFO));
    CurrentDI.lCopies   = 1;
    CurrentDI.lPriority = 50;

    /* Query the default printer */
    ulSize = PrfQueryProfileString(HINI_PROFILE,"PM_SPOOLER","QUEUE",
        0,szTemp,MAX_BUFF);

    if (ulSize)
    {
       pStr = (PSZ)strchr(szTemp,';');
       *pStr = 0;
       SplQueryQueue(0,szTemp,3,0,0,&ulNeeded);
       if (ulNeeded && !(DosSubAlloc(pMem,(PPVOID)&pPrq3,ulNeeded)))
       {
           SplQueryQueue(0,szTemp,3,pPrq3,ulNeeded,&ulNeeded);
           get_job_properties(pPrq3,(PDDINFO)&CurrentDI.lSizeDD,DPDM_QUERYJOBPROP); 
           create_info_DC(pPrq3,&CurrentDI);
           DosSubFree(pMem,pPrq3,ulNeeded);

           CurrentDI.hClientDC = WinOpenWindowDC(hWnd);
           CurrentDI.hClientPS = GpiCreatePS(hab,CurrentDI.hClientDC,
               (PSIZEL)&CurrentDI.ClientSizel,
               PU_LOMETRIC | GPIF_LONG | GPIT_NORMAL | GPIA_ASSOC);

           /* Scale the page to 50% */
           GpiQueryDefaultViewMatrix(CurrentDI.hClientPS,9,&matlfDefView );
           matlfDefView.fxM11 = MAKEFIXED(0,32768);
           matlfDefView.fxM22 = MAKEFIXED(0,32768);
           matlfDefView.lM31  = PAGE_OFFSET;
           matlfDefView.lM32  = 0;
           GpiSetDefaultViewMatrix(CurrentDI.hClientPS,9,&matlfDefView,TRANSFORM_REPLACE);

           /* Create an RGB color table */
           GpiCreateLogColorTable (CurrentDI.hClientPS,LCOL_RESET,LCOLF_RGB,0,0,NULL);
           lColorWorkSpace = WinQuerySysColor(HWND_DESKTOP,SYSCLR_APPWORKSPACE,0);
       }
    }

    if (!CurrentDI.hInfoDC)
    {
         //No printers installed, disable print menus
         WinEnableMenuItem(hwndMenu, IDM_CHOOSE_PRINTER, FALSE);
         WinEnableMenuItem(hwndMenu, IDM_PRINTER_INFO, FALSE);
         WinEnableMenuItem(hwndMenu, IDM_PRINT, FALSE);
    }
    return (CurrentDI.hInfoDC);
}


USHORT APIENTRY post_error(HWND hWnd,USHORT usErrorID)
/*-----------------------------------*\

   This is a utility function which will post a message box with
   a string loaded from the resource file.

\*-----------------------------------------------------------------*/
{
    WinLoadString (hab,0,usErrorID,MAX_BUFF,szBuff1);
    return (WinMessageBox(HWND_DESKTOP,hWnd,(PSZ)szBuff1,
        (PSZ)"ZED Error",0,MB_OK | MB_ICONHAND | MB_APPLMODAL));
}



#ifdef __IBMC__
#pragma linkage(process_print, system)
#endif

void APIENTRY process_print(HWND hPrintStatusDlg)
/*-----------------------------------*\

   This function perfoms the actual printing. A copy of the current
   device settings are copied to an allocated memory block and a
   print DC is created. If successful the printing is done and the
   job is completed. The print status dialog is updated via posted
   WM_PRINT_STATUS messages.

\*-----------------------------------------------------------------*/
{
    PDEVICEINFO pDI;
    long lRes;
    int iRes;

    if (DosSubAlloc(pMem,(PPVOID)&pDI,sizeof(DEVICEINFO)) == 0) 
    {
       *pDI = CurrentDI;

       if (create_print_DC(pDI))
       {
           WinPostMsg(hPrintStatusDlg,WM_PRINT_STATUS,(MPARAM)JOB_STARTED,0);
            //Draw the pages.
            iRes=draw_pages(pDI);
           lRes=DevEscape(pDI->hPrintDC,DEVESC_ENDDOC,0,0,0,0);
            if(lRes!=DEV_OK) WinMessageBox(HWND_DESKTOP,hwndClient,"DevEscError","",0,NULL);
           
           WinPostMsg(hPrintStatusDlg,WM_PRINT_STATUS,(MPARAM)JOB_FINISHED,0);
     
           GpiAssociate(pDI->hPrintPS,0);
           GpiDestroyPS(pDI->hPrintPS);
           DevCloseDC(pDI->hPrintDC);
       }
       else
           WinPostMsg(hPrintStatusDlg,WM_PRINT_STATUS,(MPARAM)JOB_FAILED,0);
       DosSubFree(pMem,pDI,sizeof(DEVICEINFO));
    }
}    





void APIENTRY process_size(MPARAM mp1,MPARAM mp2)
{
    POINTL   ptl;
    MATRIXLF matlfDefView; 

    CurrentDI.cx = SHORT1FROMMP(mp2);
    CurrentDI.cy = SHORT2FROMMP(mp2);
    WinSetWindowPos (hHintWnd,HWND_TOP,0,0,SHORT1FROMMP(mp2),
        nSysFontHeight,SWP_SIZE | SWP_MOVE);

    /* Reset the default view matrix before converting client size */
    ptl.x = PAGE_OFFSET;
    ptl.y = (CurrentDI.cy - nSysFontHeight) - PAGE_OFFSET + 1;
    GpiQueryDefaultViewMatrix(CurrentDI.hClientPS,9,&matlfDefView );
    matlfDefView.lM31  = 0;
    matlfDefView.lM32  = 0;
    GpiSetDefaultViewMatrix(CurrentDI.hClientPS,9,&matlfDefView,
         TRANSFORM_REPLACE);
    GpiConvert(CurrentDI.hClientPS,CVTC_DEVICE,CVTC_WORLD,1,(PPOINTL)&ptl);
    ptl.y = (ptl.y - CurrentDI.PhyPageSizel.cy) + 
        CurrentDI.PrintOffsetSizel.cy;
    ptl.x = ptl.x + CurrentDI.PrintOffsetSizel.cx;
    matlfDefView.lM31 = (LONG)((float)ptl.x * 0.5);
    matlfDefView.lM32 = (LONG)((float)ptl.y * 0.5);
    GpiSetDefaultViewMatrix(CurrentDI.hClientPS,9,&matlfDefView,
        TRANSFORM_REPLACE);
    return;
    mp1;
}


MRESULT EXPENTRY PrintDlgProc(HWND hWndDlg,ULONG ulMessage,MPARAM mp1,
                              MPARAM mp2)
/*-----------------------------------*\

   This function will display the print confirmation dialog box. It
   allows the user to verify the print destination and set the job
   priority and print copies.

\*-----------------------------------------------------------------*/
{
    HWND  hWnd;
    ULONG ulSelect;

    switch (ulMessage) 
    {
        case WM_INITDLG:
            center_window(hWndDlg);
            hWnd = WinWindowFromID(hWndDlg,DID_PRINTER);
            WinSetWindowText(hWnd,CurrentDI.szQueueDesc);
            WinSendMsg(hWnd,EM_SETREADONLY,(MPARAM)TRUE,0);

            hWnd = WinWindowFromID(hWndDlg,DID_DOCUMENT);
            
            WinSetWindowText(hWnd,szFileName);
            WinSendMsg(hWnd,EM_SETREADONLY,(MPARAM)TRUE,0);

            /* Set the limits to the valid range for a queue processor */
            hWnd = WinWindowFromID(hWndDlg,DID_COPIES);
            WinSendMsg(hWnd,SPBM_SETLIMITS,(MPARAM)999,(MPARAM)1);
            WinSendMsg(hWnd,SPBM_SETCURRENTVALUE,MPFROMLONG(CurrentDI.lCopies),0);
            WinSetFocus(HWND_DESKTOP,hWnd);

            /* Initialize the priority spinner control */
            hWnd = WinWindowFromID(hWndDlg,DID_PRIORITY);
            WinSendMsg (hWnd,SPBM_SETTEXTLIMIT, (MPARAM)9L,(MPARAM)0L);
            WinSendMsg (hWnd,SPBM_SETARRAY, (MPARAM)pszPriorities,(MPARAM)3);
            WinSendMsg (hWnd,SPBM_SETCURRENTVALUE,(MPARAM)1,(MPARAM)0L);
            return ((MRESULT)TRUE);
        case WM_COMMAND:
            switch (SHORT1FROMMP(mp1)) 
            {
                case DID_OK:
                    hWnd = WinWindowFromID(hWndDlg,DID_COPIES);
                    WinSendDlgItemMsg(hWndDlg,DID_COPIES,SPBM_QUERYVALUE,
                        &CurrentDI.lCopies,MPFROM2SHORT(0,SPBQ_UPDATEIFVALID));
                    /* Query selection index from spinner */
                    if(WinSendDlgItemMsg (hWndDlg, DID_PRIORITY,
                        SPBM_QUERYVALUE, (MPARAM)&ulSelect, 0L))
                        CurrentDI.lPriority = lPriority[ulSelect];
                    else
                        CurrentDI.lPriority = 50;
                    WinDismissDlg(hWndDlg,DID_OK);
                    return 0;
            }
            break;
        case WM_CONTROL:
            switch (SHORT1FROMMP(mp1))
            {
                case DID_PRIORITY:
                    if (HIUSHORT(mp1) == SPBN_ENDSPIN)
                    {
                        /* Query selection index from spinner */
                        if(WinSendDlgItemMsg (hWndDlg, DID_PRIORITY,
                            SPBM_QUERYVALUE, (MPARAM)&ulSelect, 0L))
                            CurrentDI.lPriority = lPriority[ulSelect];
                        else
                            CurrentDI.lPriority = 50;
                    }
                    break;
            }
            break;
    }
    return (WinDefDlgProc(hWndDlg,ulMessage,mp1,mp2));
}



MRESULT EXPENTRY PrintStatusProc(HWND hWndDlg,ULONG ulMessage,
                                 MPARAM mp1,MPARAM mp2)
/*-----------------------------------*\

   This function is the callback for the print job status dialog.
   When the WM_INITDLG message is received a WM_START_PRINT message
   is posted to the client window requesting the print job to be started.
   When the WM_PRINT_STATUS message is received the text in the status
   window is updated accordingly.

\*-----------------------------------------------------------------*/
{
    HWND  hWnd;
    LONG  lColor;

    switch (ulMessage) 
    {
        case WM_INITDLG:
            center_window(hWndDlg);
            hWnd = WinWindowFromID(hWndDlg,DID_OK);
            WinShowWindow(hWnd,FALSE);

            hWnd = WinWindowFromID(hWndDlg,DID_JOBNAME);
            //WinLoadString(hab,0,IDS_JOB_TITLE,sizeof(szBuff1),szBuff1);
            strcpy(szBuff1,szFileName);
            WinSetWindowText(hWnd,szBuff1);

            hWnd = WinWindowFromID(hWndDlg,DID_PRINTER);
            WinSetWindowText(hWnd,CurrentDI.szQueueDesc);

            hWnd = WinWindowFromID(hWndDlg,DID_STATUS);
            WinSetWindowText(hWnd,"Initializing Printer");

            WinPostMsg(hwndClient,WM_START_PRINT,(MPARAM)hWndDlg,
               (MPARAM)*(PULONG)mp2);
            break;
        case WM_PRINT_STATUS:
            hWnd = WinWindowFromID(hWndDlg,DID_STATUS);
            switch(SHORT1FROMMP(mp1))
            {
                case JOB_STARTED:
                    WinSetWindowText(hWnd,"Starting Print Job");
                    break;
                case JOB_CANCELED:
                    WinSetWindowText(hWnd,"Print Job Cancelled");
                    WinPostMsg(hWndDlg,WM_COMMAND,(MPARAM)DID_OK,0);
                    break;
                case JOB_FAILED:
                    lColor = CLR_RED;
                    WinSetPresParam(hWnd,PP_FOREGROUNDCOLORINDEX,sizeof(PLONG),&lColor);
                    WinSetWindowText(hWnd,"Print Job Failed");
                    WinAlarm(HWND_DESKTOP,WA_ERROR);
                    /* Switch the cancel button to an ok button */
                    hWnd = WinWindowFromID(hWndDlg,DID_OK);
                    WinShowWindow(hWnd,TRUE);
                    hWnd = WinWindowFromID(hWndDlg,DID_CANCEL);
                    WinShowWindow(hWnd,FALSE);
                    break;
                case JOB_FINISHED:
                    WinSetWindowText(hWnd,"Print Job Finished");
                    WinPostMsg(hWndDlg,WM_COMMAND,(MPARAM)DID_OK,0);
                    break;
            }
            break;
        case WM_COMMAND:
            switch (SHORT1FROMMP(mp1)) 
            {
                case DID_CANCEL:
                    hWnd = WinWindowFromID(hWndDlg,DID_STATUS);
                    WinSetWindowText(hWnd,"Canceling Print Job");
                    WinPostMsg(hwndClient,WM_ABORT_PRINT,0,0);
                    return 0;
                case DID_OK:
                    WinDismissDlg(hWndDlg,DID_OK);
                    return 0;
            }
            break;
    }
    return (WinDefDlgProc(hWndDlg,ulMessage,mp1,mp2));
}


MRESULT EXPENTRY SelectPrinterDlgProc(HWND hWndDlg,ULONG ulMessage,
                                      MPARAM mp1,MPARAM mp2)
/*-----------------------------------*\

   This function is the callback for the printer selection dialog.
   It will fill a list box with installed queue entries and post
   job properties when requested. If OK is selected the current
   queue selection along with it's last job properties are set in
   the CurrentDI variable.

\*-----------------------------------------------------------------*/
{
    static  PPRQINFO3 pQueueInfo;
    static  ULONG     ulSize = 0;
    static  HWND      hWndLBox;
    static  BOOL      bRedraw = FALSE;
    static  PDDINFO   pDDInfo;
    static  ULONG     ulCount;
    RECTL   Rectl;
    int     i;
    PDDINFO pDDI;
    SHORT   sSelection;
    ULONG   ulCurrent;

    switch (ulMessage) 
    {
        case WM_INITDLG:
            center_window(hWndDlg);
            hWndLBox = WinWindowFromID(hWndDlg,DID_QUEUE_NAMES);
            if (!(pQueueInfo = enum_print_queues(hWndDlg,hWndLBox,
                (PULONG)&ulSize,(PULONG)&ulCount,(PULONG)&ulCurrent)))
               WinDismissDlg(hWndDlg,DID_OK);
            else if (!DosSubAlloc(pMem,(PPVOID)&pDDInfo,ulCount * sizeof(DDINFO)))
            {
               memset(pDDInfo,0,ulCount * sizeof(DDINFO));

               /* Preload the entry for the current printer with the 
                  saved values from CurrentDI */
               (pDDInfo + ulCurrent)->pDrivData = CurrentDI.pDrivData;
               (pDDInfo + ulCurrent)->lSizeDD = CurrentDI.lSizeDD;

               /* Select the current entry in the list box */
               WinSendMsg (hWndLBox,LM_SELECTITEM,MPFROMSHORT(ulCurrent),MPFROMSHORT(TRUE));
            }
            break;
        case WM_COMMAND:
            switch (SHORT1FROMMP(mp1)) 
            {
                case DID_OK:
                    sSelection = (USHORT)WinSendMsg (hWndLBox,LM_QUERYSELECTION,
                       MPFROMSHORT(LIT_FIRST),0);
                    if (!(pDDInfo + sSelection)->pDrivData)
                    {
                        get_job_properties(&pQueueInfo[sSelection],
                            &pDDInfo[sSelection],DPDM_QUERYJOBPROP); 
                        bRedraw = TRUE;
                    }
                    if ((pDDInfo + sSelection)->pDrivData)
                    {
                        /* If we are changing application default printers
                           free up current driver data and replace it with
                           the driver data for the selected printer. */
                        if ( CurrentDI.pDrivData && 
                            (CurrentDI.pDrivData != (pDDInfo + sSelection)->pDrivData) )
                        {
                            DosSubFree(pMem,CurrentDI.pDrivData,CurrentDI.lSizeDD);
                            CurrentDI.pDrivData = (pDDInfo + sSelection)->pDrivData;
                            CurrentDI.lSizeDD = (pDDInfo + sSelection)->lSizeDD;
                        }
                        (pDDInfo + sSelection)->pDrivData = 0;
                        (pDDInfo + sSelection)->lSizeDD = 0;
                    }
                    create_info_DC(&pQueueInfo[sSelection],(PDEVICEINFO)&CurrentDI); 
                    WinDismissDlg(hWndDlg,DID_OK);

                    pDDI = pDDInfo;
                    for (i = 0; i < (int)ulCount;i++)
                    {
                        if ((pDDI + i)->pDrivData)
                            DosSubFree(pMem,(pDDI + i)->pDrivData,
                                (pDDI + i)->lSizeDD);
                    }
                    DosSubFree(pMem,pDDInfo,ulCount * sizeof(DDINFO));
                    DosSubFree(pMem,pQueueInfo,ulSize);
                    if (bRedraw)
                    {
                        WinQueryWindowRect(hwndClient,(PRECTL)&Rectl);
                        WinSendMsg(hwndClient,WM_SIZE,0,
                            MPFROM2SHORT(Rectl.xRight,Rectl.yTop));
                        WinInvalidateRect(hwndClient,0,TRUE);
                    }
                    break;
                case DID_CANCEL:
                    WinDismissDlg(hWndDlg,DID_CANCEL);
                    pDDI = pDDInfo;
                    for (i = 0; i < (int)ulCount;i++)
                    {
                        /* Free up any allocated driver data except the
                           current application default  */
                        if ( (pDDI + i)->pDrivData &&
                             (CurrentDI.pDrivData != (pDDI + i)->pDrivData) )
                            DosSubFree(pMem,(pDDI + i)->pDrivData,
                                (pDDI + i)->lSizeDD);
                    }
                    DosSubFree(pMem,pDDInfo,ulCount * sizeof(DDINFO));
                    DosSubFree(pMem,pQueueInfo,ulSize);
                    break;
                case DID_JOBPROPS:
                    sSelection = (USHORT)WinSendMsg (hWndLBox,LM_QUERYSELECTION,
                       MPFROMSHORT(LIT_FIRST),0);
                    if (sSelection != LIT_NONE)
                    {
                        get_job_properties(&pQueueInfo[sSelection],
                            &pDDInfo[sSelection],DPDM_POSTJOBPROP); 
                        bRedraw = TRUE;
                    }
                    return 0;
            }
            break;
        default:
            break;
    }
    return (WinDefDlgProc(hWndDlg,ulMessage,mp1,mp2));
}
