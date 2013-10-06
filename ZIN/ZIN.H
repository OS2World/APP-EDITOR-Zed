///////////////////////////////////////////////////////////
// ZIN.H
// Header file for ZIN installation program.
//
// History:
//    Created:    04/09/95    1.0    Bruce Shankle
///////////////////////////////////////////////////////////

#ifdef MAIN
#define GLOBAL
#else
#define GLOBAL extern
#endif

//Defines
#define MAXBUF    512
#define LITTLEBUF 80

//Global variables
GLOBAL HAB hab;
GLOBAL HMQ hmq;
GLOBAL HWND hwndFrame, hwndClient;
GLOBAL QMSG qmsg;
GLOBAL CHAR szBuffer[MAXBUF];
GLOBAL CHAR szAppName[LITTLEBUF];
GLOBAL CHAR szInstallDir[CCHMAXPATH];
GLOBAL CHAR szSourceDir[CCHMAXPATH];
GLOBAL USHORT usResponse;



//Function prototypes
GLOBAL MRESULT EXPENTRY ClientWndProc(HWND, ULONG, MPARAM, MPARAM);
GLOBAL void PaintClient (HPS, SHORT, SHORT);
GLOBAL MRESULT EXPENTRY GetDirDlgProc (HWND, ULONG, MPARAM, MPARAM); //GETDIR.C
GLOBAL MRESULT EXPENTRY ReadmeDlgProc (HWND, ULONG, MPARAM, MPARAM); //RM.C

GLOBAL USHORT MsgBox (ULONG, ULONG, ULONG); //MSG.C
GLOBAL APIRET CheckPath (char * szPath);
GLOBAL APIRET CreateDirectory(char * szPath);
GLOBAL GetCurrentDirectory(char * szCurrentDirectory);

//Status.c declares
GLOBAL MRESULT EXPENTRY StatusDlgProc (HWND hwnd, 
                                        ULONG msg,
                                        MPARAM mp1, 
                                        MPARAM mp2);

void LoadStatusDlg(void);
void DestroyStatusDlg(void);
void SetStatus (ULONG ulMessageID);

