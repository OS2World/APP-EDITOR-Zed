/////////////////////////////////////////////
//AUTOSAVE.H
//HEADER file for autosave dialog
//
//Created:    02/14/1995    Bruce Shankle

#ifdef AUTOSAVE_MAIN       
#define AUTOSAVEGLOBAL
#else
#define AUTOSAVEGLOBAL extern
#endif

#define IDD_AUTOSAVE                850
#define ID_CHK_AUTOSAVEMINUTES   851
#define ID_SPIN_AUTOSAVEMINUTES     852
#define ID_CHK_AUTOSAVEMODIFICATIONS 853
#define ID_SPIN_AUTOSAVEMODIFICATIONS 854
#define ID_CHK_AUTOSAVEPROMPTFORNAME 855
#define IDB_AUTOSAVEOK              856
#define IDB_AUTOSAVECANCEL         857
#define IDB_AUTOSAVEHELP            858

//AutosaveTimerProc Actions
#define AUTOSAVETIMER_INITIALIZE    0
#define AUTOSAVETIMER_INCREMENT    1
#define AUTOSAVEMOD_INITIALIZE    2
#define AUTOSAVEMOD_INCREMENT   3

AUTOSAVEGLOBAL long glAutoSaveMinutes;
AUTOSAVEGLOBAL long glAutoSaveModifications;
AUTOSAVEGLOBAL BOOL gbUseAutoSaveMinutes;
AUTOSAVEGLOBAL BOOL gbUseAutoSaveModifications;
AUTOSAVEGLOBAL BOOL gbUseAutoSavePromptForName;

//Function declarations
MRESULT EXPENTRY AutoSaveDlgProc(HWND, ULONG, MPARAM, MPARAM);
int AutoSaveTimerProc(int Action);
int AutoSaveModProc(int Action);
int AutoSaveFile(void);
void GetDirectory(char * szDir);


