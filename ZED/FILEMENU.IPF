.********************************************
.*FILEMENU.IPF
.*This file contains ZED file menu help.
.*It is intended to be imbedded in MENUS.IPF
.*Created : 02/22/1995 Bruce Shankle
.********************************************

:h2 res=3012.File Menu
:p.
The following selections are available in the "File" menu.

:p.
:hp2.New:ehp2. - edit new of different file.
.br
Clear text from edit window and close current file. If the current text is
modified and not saved, ZED will ask if you want to save it.  This menu
item is disabled if the current text is empty.

:p.
:hp2.Open - Ctrl+O:ehp2. - open a file.
.br
Bring up file open dialog window. Select a file, and ZED will open it and you can
edit it.

:p.
:hp2.Import - Ctrl+I:ehp2. - import a file.
.br
Bring up file import dialog window. Select a file to import into the current text.
The text in the file will be inserted at the current cursor location.

:p.
:hp2.Save - F2:ehp2. - save current text.
.br
Save the current text in a file. If the text is untitled, open save dialog window
to assign the file a directory and name.  This menu item is disabled if
the current text has not been modified.

:p.
:hp2.Save as:ehp2. - save current text in a new or different file.

:p.
:hp2.Save and exit - F4:ehp2. - save current text and exit.
.br
Save the current text in a file and exit. If the text is untitled, open save dialog window
to assign the file a directory and name.  If the text has a file name, save text to that file 
and then exit.  This menu item is disabled if the current text has not
been modified.

:p.
:hp2.Select printer...:ehp2. - select target print object.
.br
Select printer object to recieve printed text.  Optionally set job properties.

:p.
:hp2.Printer information...:ehp2. - view detailed printer information.
.br
Show detailed information about each installed printer object.

:p.
:hp2.Print...:ehp2. - print text.
.br
Send text to printer.  This opens a dialog window that allows you to set
priority, copies, and job properties before actually printing.  If a line of
text exceeds the width of the print area, it is truncated.

:p.
:hp2.Spawn ZED:ehp2. - start another program instance.
.br
Start a new copy of ZED.

:p.
:hp2.Recent file list:ehp2. - quickly open a previously opened file.
.br
ZED allows up to 9 file names in the recent file list.  These file names are stored in
when you exit ZED or select "Save settings" from the settings menu.  When you open a file
it's name is added to the recent file list. If the file is already in the list, it is brought to
the top of the list.  If the file list is full, ZED removes the last entry to make room
for the new entry.

:p.
:hp2.Exit - F3:ehp2. - exit ZED.
.br
Close ZED.  Prompt to save any modified text.
.br



.***********************************
.*Links
.*Link back to menu help
.br
:link reftype=hd res=3013.Menus:elink.

.*Link back to general help.
.br
:link reftype=hd res=657.General Help:elink.

.*************************************
.*Indexing
:i1.New
:i1.Open
:i1.Save
:i1.Save as
:i1.Save and exit
:i1.Spawn ZED
:i1.Recent file list
:i1.Exit
:i1.Print
:i1.Printer select
:i1.Printer information
:i1.Importing a file

.*End font help
.**********************************