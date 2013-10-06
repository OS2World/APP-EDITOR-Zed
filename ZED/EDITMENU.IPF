.********************************************
.*EDITMENU.IPF
.*This file contains ZED edit menu help.
.*It is intended to be imbedded in MENUS.IPF
.*Created : 02/22/1995 Bruce Shankle
.********************************************

:h2 res=3014.Edit Menu
:p.
The following selections are available in the "Edit" menu.
.br
Note: Some options are not always available.  For example, "Paste" is disabled
if there is not text in the clipboard.

:p.
:hp2.Undo - Ctrl+Z:ehp2. - undo last operation.
.br
This allows most text changes to be undone, such as deleting or inserting something.
It also allows color and font changes to be undone.  The undo operation, cannot undo
some operations, such as deleting a large amount of text.  Undo only undoes the last
operation.

:p.
:hp2.Cut - Ctrl+X:ehp2. - cut selected text and place in clipboard.
.br
This menu item is disabled if no text is selected.

:p.
:hp2.Copy - Ctrl+X:ehp2. - copy selected text to clipboard.
.br
This menu item is disabled if no text is selected.  The Ctrl+Insert key sequence
also activates this function.

:p.
:hp2.Paste - Ctrl+V:ehp2. - insert text from clipboard at current cursor location.
.br
This menu item is disabled if no text is in the clipboard.  The Shift+Insert key 
sequence also activates this function.

:p.
:hp2.Delete - Del:ehp2. - delete selected text.
.br
This menu item is disabled if no text is selected.


:p.
:hp2.Find... - Ctrl+F:ehp2. - show find dialog window.
.br
When the find dialog window is displayed, text can be entered and ZED will search for
that text.  If the text is found, it will be selected and scrolled into view.  If it is not 
found, ZED will issue a message.  This menu item is disabled if there is no text.

:p.
:hp2.Find next - F5:ehp2. - find previously searched text again.
.br
Search for the next instance of the previously searched text, automatically wrapping to 
the beginning if it reaches the end of the text.  If the text is found, it will be selected and
scrolled into view.  If it is not found, ZED will issue a message. This menu
item is disabled if there is no text.

:p.
:hp2.Replace - Ctrl+R:ehp2. - show replace dialog window.
.br
When the replace dialog window is displayed, search and replace text can be entered.
ZED will replace the search text with the replace text.  You have the option to verify each
replace operation or allow ZED to replace all instances of the search text.  If the text
cannot be found, ZED will issue a message.  This menu item is disabled if there is no
text.
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
:i1.Edit menu
:i1.Undo
:i1.Cut
:i1.Copy
:i1.Paste
:i1.Delete
:i1.Find
:i1.Find next
:i1.Replace
:i1.Verify

.*End font help
.**********************************