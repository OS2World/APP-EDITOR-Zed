.********************************************
.*AUTOSAVE.IPF
.*This file contains ZED autosave help.
.*It is intended to be imbedded in ZED.IPF
.*Created : 02/22/1995 Bruce Shankle
.********************************************

:h1 res=3006.Using auto-save
:p.
ZED provides auto-save features to help insure your text is not
lost in the event of a power outage or system failure.

:p.
Auto-save can
be configured to save based on elapsed time and/or based on the number
of modifications to the text.  Auto-save can also generate a file name for
an untitled document or prompt for a file name, depending on how you 
configure the auto-save settings.  ZED does not auto-save text that has
not been modified.

:p.
ZED Auto-save settings are configured in the ZED AutoSave dialog window.
To activate this window, select "Auto-save..." from the "Settings" menu.

:p.
:hp2.Automatic save every XX minutes:ehp2.
.br
Check this option to automatically save your document
every time the specified time passes. The time interval, represented by the "XX"
above, is in minutes and can be from 1 to 60.

:p.
:hp2.Automatic save every XXXX modifications:ehp2.
.br
Check this option to automatically save your document after the specified
number of modifications.  The number of modifications, represented by the "XXXX",
is the number of key presses that modifiy text in ZED's edit window.  This value can
be from 1 to 5000.

:p.
:hp2.Prompt for name if untitled:ehp2.
.br
Check this option if you want ZED to prompt you for a file name when it needs to
auto-save untitled text. If this option is not checked, ZED will automatically generate
a file name for the text: i.e. AUTOSAVE.001.
.br
Automatically generated file names will range from AUTOSAVE.001 
to AUTOSAVE.999 and will be located in the directory where ZED.EXE is located.
If the maximum number of auto-generated file names is reached, ZED will issue 
a warning and the auto-save operation will fail.  If this happens, it is best to delete
some of the auto-saved files or enable this option.

:p.
When you are done, click the "OK" button.  The settings will
be saved when you exit ZED or select "Save settings" from
the "Settings" menu.
.br
Click the "Cancel" button to cancel any changes.


:p.
If auto-save fails, insure that you enough free storage space to save the text.
Also make sure that you are not running ZED from read only media like CD-ROM or
a network server where you don't have write permission.

.*********************************
.**Hyper Links

.*Link back to settings menu help
.br
:link reftype=hd res=3011.Settings menu:elink.

.*Link back to general help.
.br
:link reftype=hd res=657.General Help:elink.

.*Link to AutoSave message box - prompt for file name.
.br
:link reftype=hd res=3007.AutoSave message box - prompt for file name:elink.

.*Link to warning message box
.br
:link reftype=hd res=3008.AutoSave message box - Warning:elink.
.*End links for AUTOSAVE.IPF heading 1.
.*********************************


.*********************************
.*Heading 2 
:h2 res=3007.AutoSave message box - prompt for file name
:p.
A message box is displayed when AutoSave is configured to prompt for a filename.
Clicking the "OK" button in this message box 
will bring up the "Save" dialog window to allow you
to give the current text a file name.
.br
:link reftype=hd res=3006.Auto-save help:elink.

:h2 res=3008.AutoSave message box - Warning
:p.
If you fail to supply a file name in the "Save" window when AutoSave prompts
for one, you will be issued this warning. This is just a warning,
but you will recieve this warning
each time auto-save activates and you don't supply a file name when it prompts
for one.
.br
:link reftype=hd res=3006.Auto-save help:elink.

.*End AUTOSAVE.IPF Heading 2 
.*********************************

.*********************************
.*Indexing
:i1.Auto-save

.*End AUTOSAVE.IPF
.********************************************