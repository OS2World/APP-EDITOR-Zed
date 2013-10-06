.********************************************
.*ALPHATST.IPF
.*This file contains ZED Alpha tester info.
.*It is intended to be imbedded in ZED.IPF
.*Created : 02/22/1995 Bruce Shankle
.********************************************
:h1 res=3003.Alpha tester documentation

.br
:hp2.IMPORTANT - PLEASE READ:ehp2.
.br
:p.Updated: 03/07/1995    by Bruce Shankle
:p.If this is your first time running ZED Alpha 10, this window should have popped up
automatically.  It will only do this once (unless you delete ZED.INI).

:p.For best results with ZED Alpha 10, I recommend you delete the old ZED.INI. It should
be located in the directory where ZED is.

:p.ZED has some new features and bug-fixes in this release:

:p.
:hp2.Printing:ehp2.
.br
Printing is here.  Keep in mind this is the first release of ZED with printing.
I would appreciate feed-back on how ZED interacts with your specific printer.
If you have any problems, please describe it in detail and include the type
of printer you have.
.br
ZED currently uses the font that the system assigns it when ZED requests a
printer presentation space.  The next release will use the font used in the 
edit window.
.br
If a line of text is too long, it will be truncated.

:p.
:hp2.Fixes / enhancements:ehp2.
.br
Auto-Indent was causing the text to revert to black on some video cards under
OS/2 Warp.
I believe I have fixed this problem.

:p.
The recent file list now works will all recent files.

:p.
Certain file menu options are now disabled when they are not
appropriate: i.e. if the text is not modified, you can't save it, but you
can still use "Save as".  If any of these disabled menu items are a problem
let me know.

:p.
At high resolutions, the "MODIFIED" text in the status bar was extending
beyond it's little box. This has been fixed.

:p.
Ctrl+X keys were not working when caps lock was down, this has been fixed.

.br


:link reftype=hd res=3002.License Agreement:elink.
.br

.*End Alpha tester documentation
.***********************************************
