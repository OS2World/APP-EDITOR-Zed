////////////////////////////////////////////////////////////////////////////
//RCNTFILE.H
//Copyright (C) 1995 by Bruce Shankle
//
//ZED Text Recent file functions header file
//
//Created :       02/11/1995 by Bruce Shankle
//
////////////////////////////////////////////////////////////////////////////

int AddRecentFile(char * szFilename);
int AddToList(char * szFilename);
int BringToTop(char * szFilename);
int UpdateRecentFileMenu(void);

