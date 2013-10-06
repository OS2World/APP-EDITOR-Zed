/*This command file installs ZED on your desktop. */

Call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
Call SysLoadFuncs
Say "ZED Installation script Version 1.0s"
Say "by E. Bruce Shankle III"
Say

'@ECHO OFF'

CURDIR=Directory()

Parse Upper Value CURDIR With CURDIR
EXENAME=CURDIR||"\ZED.EXE"
ICONFILE=CURDIR||"\ZED.ICO"

PRGOPTION="PROGTYPE=PM;EXENAME="||EXENAME||";PARAMETERS=;"
PRGOPTION=PRGOPTION||"ICONFILE="||ICONFILE
'@ECHO OFF'
if SysCreateObject("WPProgram", ,
                  "ZED 1.0s", ,
                  "<WP_DESKTOP>", ,
                  PRGOPTION, ,
                  "REPLACE") then
ZED register.txt
else
say "Could not created ZED 1.0s object on desktop."
                  
exit



