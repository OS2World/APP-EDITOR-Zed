////////////////////////////////////////////////////
//PALETTE.H
//Header file for PALETTE.C
//Created:	02/04/1995	Bruce Shankle

#ifdef PALETTE_MAIN
#define PALETTE_GLOBAL
#else
#define PALETTE_GLOBAL extern
#endif

PALETTE_GLOBAL HOBJECT hFontPalette;
PALETTE_GLOBAL HOBJECT hColorPalette;
PALETTE_GLOBAL HWND hwndFontPalette;
PALETTE_GLOBAL HWND hwndColorPalette;


//Function for handling font changes.
void FontPalProc(void);
void ColorPalProc(void);
