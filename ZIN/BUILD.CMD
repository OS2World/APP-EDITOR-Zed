@echo off
echo Building distribution files.
c:
cd\
md dist
cd dist
echo Deleting old files...
echo y | del .
d:
cd\
cd progs
cd os2
cd zed
echo Making help
call makehelp.cmd
echo Making ZED_11S.ZIP...
pkzip c:\dist\ZED_11S.ZIP ZED.EXE ZED.HLP ZED.ICO README.TXT
cd ..
cd zin
echo Adding install program to ZED_11S.ZIP...
pkzip c:\dist\ZED_11S.ZIP ZIN.EXE
cd ..
cd zed
echo Copying files...
copy zed.exe c:\dist\ZED.EXE
copy zed.hlp c:\dist\ZED.HLP
copy zed.ico c:\dist\ZED.ICO
copy readme.txt c:\dist\README.TXT
cd ..
cd zin
echo Copying files...
copy zin.exe C:\dist\INSTALL.EXE

echo End of build.

