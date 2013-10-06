////////////////////////////////////////////////////////////
//INI.H
//Header file for ini file functions

#ifdef INI_MAIN
#define INI_GLOBAL
#else
#define INI_GLOBAL extern
#endif

INI_GLOBAL HINI hIni;    //Handle to ini file.

//////////////////////////////////////////////////
//Variables retrieve from INI file
INI_GLOBAL BOOL bUseIniSettings; //Flag to know whether or not to use
                      //INI settings
INI_GLOBAL LONG INIBackColor;      //Background color for MLE
INI_GLOBAL LONG INITextColor;       //Text color for MLE
INI_GLOBAL FATTRS INIfattrs;  //MLE font attributes
INI_GLOBAL RECTL INIrcl;
INI_GLOBAL BOOL INIbWordWrap;
INI_GLOBAL char szRecentFile1[CCHMAXPATH];
INI_GLOBAL char szRecentFile2[CCHMAXPATH];
INI_GLOBAL char szRecentFile3[CCHMAXPATH];
INI_GLOBAL char szRecentFile4[CCHMAXPATH];
INI_GLOBAL char szRecentFile5[CCHMAXPATH];
INI_GLOBAL char szRecentFile6[CCHMAXPATH];
INI_GLOBAL char szRecentFile7[CCHMAXPATH];
INI_GLOBAL char szRecentFile8[CCHMAXPATH];
INI_GLOBAL char szRecentFile9[CCHMAXPATH];


//Function prototypes.
void GetINIFileName(void);
int SaveProfile (void);
BOOL GetProfile();
int DisplayINIError(void);

