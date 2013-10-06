///////////////////////////////////////////////////////////////
//HELP.H
//Header file for HELP.C
//Created:     02/20/1995    Bruce Shankle

#ifdef HELP_MAIN
#define HELP_GLOBAL static
#else
#define HELP_GLOBAL extern
#endif

//Help topics, these numbers come from the ZED.IPF help file
#define HTOPIC_ALPHADOCS 3003
#define HTOPIC_AUTOSAVE 3006
#define HTOPIC_AUTOSAVEWARNING1 3007
#define HTOPIC_AUTOSAVEWARNING2 3008
#define HTOPIC_COLORS    2000
#define HTOPIC_TABS 3009
#define HTOPIC_COPYRIGHT 3001
#define HTOPIC_REGISTRATION


//Variables 
HELP_GLOBAL HELPINIT hiInit;
HELP_GLOBAL HWND hwndHelp;

//Functions decls
int InitializeHelp(void);
int DestroyHelp(void);
int DisableHelp(void);

void HelpAutoSave(void);
void HelpUsingHelp(void);
void HelpIndex(void);
void HelpGeneral(void);
void HelpKeys(void);
void HelpContents(void);
void HelpFirstTime(void);
void HelpColors(void);
void HelpTabs(void);
BOOL EXPENTRY helpHook ( HAB habAnchor,
                          SHORT sMode,
                          USHORT usTopic,
                          USHORT usSubTopic,
                          PRECTL prclPos ) ;

 
  
  



