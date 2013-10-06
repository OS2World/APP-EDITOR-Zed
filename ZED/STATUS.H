///////////////////////////////////////////////////////
//STATUS.H
//Header file for status window and related functions
//
//Created:    01/29/1995    Bruce Shankle


//Constants
#define STATUSWND_HEIGHT 20     //Height of status window
#define ID_STATUS 1010          //ID of status window

//Function declarations
//Status window procedure declaration
MRESULT EXPENTRY StatusWndProc(HWND,ULONG,MPARAM,MPARAM);

void CreateStatusWindow(void);
void Draw3DBox(long x1, long y1, long x2, long y2, HPS hps);
void DrawString(long x, long y, HPS hps, char * szString);
void UpdateStatusWindow(void);
void ResetLineColumnStatus(void);
void UpdateKeyStatus(void);
void StatusTimerProc(void);
void TurnStatusBarOn(void);
void TurnStatusBarOff(void);


#ifdef STATUS_DOT_C
#define STATUSVAR
#else
#define STATUSVAR extern
#endif


//Variables
STATUSVAR HWND hwndStatus;
STATUSVAR long lGlobalCurrentLine;
STATUSVAR long lGlobalCurrentColumn;
STATUSVAR long bInsert;
STATUSVAR long bCapsLock;
STATUSVAR long bNumLock;
STATUSVAR long bModified;
STATUSVAR long bStatusBarOn;

