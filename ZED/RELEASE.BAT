del zed_10s.zip
del zed_10s.exe
pkzip zed_10s.zip zed.exe zed.ico zed.hlp readme.txt install.cmd register.txt
zip2exe -j zed_10s.zip
del zed_10s.zip
c:
cd\
md test
cd test
del c:\test\*.*
d:
copy zed_10s.exe c:\test
c:
pause

