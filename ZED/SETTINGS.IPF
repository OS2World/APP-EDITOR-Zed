.********************************************
.*SETTINGS.IPF
.*This file contains ZED general settings help.
.*It is intended to be imbedded in ZED.IPF
.*Created : 02/22/1995 Bruce Shankle
.********************************************

:h1 res=3011.Settings
:p.
The following settings are located in the "Settings" menu.  To toggle a
setting select it from the menu or use the appropriate keys.
If a setting is checked that setting is enabled.
If it is not checked then it is disabled.

:p.
:hp2.Auto-indent - Ctrl+A:ehp2.
.br
Enable or disable auto-indent.
.br
Auto-indent automatically inserts spaces or tab characters after the enter key is
pressed to align it with the previous text.  If the previous line contains no text
then the new line is not indented.

:p.
:hp2.Case-sensitive filenames - Ctrl+E:ehp2.
.br
Enable or disable case-sensitive file names.
.br
This setting determines whether or not ZED oberves case in file names.  If
enabled, ZED will not convert the case of file names. If disabled, ZED will convert all
filenames to upper case.  If you have a file system that supports case sensitive file names,
such as HPFS, you may wish to enable this setting.

:p.
:hp2.Status bar - Ctrl+S:ehp2.
.br
The status bar contains useful information: current line and column, key-states,
text modification status, and current time.
When editing very large files, performance will be increased by disabling the
status bar.

:p.
:hp2.Word wrap - Ctrl+W:ehp2.
.br
When word wrap is enabled, all text
is truncated to fit within the edit window.  This does not add or removed characters from
the text.  When word wrap is disabled, text may extend beyond the current window.

.*Link to autosave help in AUTOSAVE.IPF
:p.
:hp2.Auto-save... :ehp2.:link reftype=hd res=3006.Using auto-save:elink.
.br

.*Link to colors help in COLORS.IPF
:p.
:hp2.Color... :ehp2.:link reftype=hd res=2000.Changing colors:elink.

.*Link to font help in FONT.IPF
:p.
:hp2.Font... :ehp2.:link reftype=hd res=3010.Changing fonts:elink.

.*Link to tab help in TAB.IPF
:p.
:hp2.Tabs... :ehp2.:link reftype=hd res=3009.Tab Settings:elink.

.br

.*Link back to menu help
.br
:link reftype=hd res=3013.Menus:elink.

.*Link back to general help.
.br
:link reftype=hd res=657.General Help:elink.
.*End font help
.**********************************