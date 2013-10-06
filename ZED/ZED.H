////////////////////////////////////////////////////////////////////////////////
//ZED.H
//Header file for ZED text editor.
//
//Created:        01/01/1995      Bruce Shankle
//

/////////////////////////Window Identifiers
#define ID_MLE 1000     //Edit Window


/////////////////////////Constants
#define STACKSIZE                       4096

/////////////////////////File error constants
#define ERROR_FILE_NOT_FOUND            2
#define ERROR_PATH_NOT_FOUND            3
#define ERROR_ACCESS_DENIED             5
#define ERROR_SHARING_VIOLATION         32


//////////////////////////Structures
typedef struct { HWND hwnd; } THREADPARAM;  //structure to pass to new threads.
typedef THREADPARAM * PTHREADPARAM;


//////////////////////////Function prototypes
int FileOpenProc (HWND hwnd, char * szOpenFileName);    //FILEOPEN.C
void FileNewProc (HWND hwnd);                           //FILENEW.C
void FileSaveAsProc (HWND hwnd);                        //FILESVAS.C
void FileSaveProc (HWND hwnd);                          //FILSAVE.C
void ReadFile(char * szInfile);                         //READFILE.C
int SaveFile(char * szOutfile);                         //SAVEFILE.C
void FindNextProc (void);
MRESULT EXPENTRY AboutDlgProc (HWND, ULONG, MPARAM, MPARAM); //ABOUT.C
MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, 
                                MPARAM mp1, MPARAM mp2); //BEDIT.C
MRESULT EXPENTRY FindDlgProc (HWND, ULONG, MPARAM, MPARAM); //FIND.C
MRESULT EXPENTRY ReplaceDlgProc(HWND, ULONG, MPARAM, MPARAM); //REPLACE.C
MRESULT EXPENTRY VerifyDlgProc(HWND, ULONG, MPARAM, MPARAM); //VERIFY.C
MRESULT EXPENTRY TabSetDlgProc(HWND, ULONG, MPARAM, MPARAM); //TAB.C
MRESULT EXPENTRY MLECustomProc(HWND, ULONG, MPARAM, MPARAM); //BEDIT.C

void EnableFileMenu(BOOL enable);                       //ZED.C
void TabProc(VOID);                                     //TAB.C

//Define global as extern or not.
//In every file except ZED.C, all globals must be defined as
//external for ANSI compatibility.
#ifdef ZED_MAIN       
#define GLOBAL
#else
#define GLOBAL extern
#endif

/////////////////////////Global variables
GLOBAL HWND hwndClient;                        //Client window handle
GLOBAL HWND hwndFrame;                         //Frame window handle
GLOBAL HWND hwndMLE;                           //Multi-line edit window handle
GLOBAL HWND hwndMenu;                          //Handle to menu
GLOBAL char szTitleBuffer[CCHMAXPATH+5];       //Buffer to hold window title
GLOBAL char szFileName[CCHMAXPATH];            //Buffer to hold open file name
GLOBAL char szCurrentDirectory[CCHMAXPATH+5];  //Buffer to hold current directory
GLOBAL HAB hab;                                //Anchor block handle
GLOBAL HMQ hmq;                                //Message queue handle     
GLOBAL TID ReadThreadID;                       //ReadFile threadID.
GLOBAL char buffer[256];			//Buffer to hold text messages.
GLOBAL char szInstDate[25];                    //Buffer to hold install date
GLOBAL char szInstInfo[25];
GLOBAL int iContinueOpen;                      //For multi-threaded file-open
GLOBAL int iReadFileFinished;                   //For communicating with ReadFile thread.
GLOBAL char szGlobalSearchString[256];          //To hold most recently searched string
GLOBAL char szGlobalReplaceString[256];         //
GLOBAL int iGlobalNumTabSpaces;
GLOBAL int iGlobalUseTabCharacter;
GLOBAL ULONG ulGlobalCaseSensitive;             //To hold case-sensitive flag
GLOBAL BOOL bUpdateTitle;                        //Flag for changing window title.
GLOBAL BOOL bInsertTabs;                        //Flag for inserting Tab characters
GLOBAL int iSpaceTabSize;                       //Size of space-tabs.
GLOBAL PFNWP MLEDefProc;//, MLECustomProc;         //For sub-classing MLE Window
GLOBAL BOOL gbCaseSensitiveFileNames;    //Use or not use case-sensitive file names (for HPFS)


//GLOBAL ATOM atSavePath;         //For file dialog saving path

//GLOBAL HATOMTBL hAtomTable;     //For file dialog saving path

//Define szAppName
#ifdef ZED_MAIN
char szAppName[]="ZED";                //Applicaiton name
#else
extern char szAppName[5];
#endif


