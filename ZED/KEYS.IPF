.********************************************
.*KEYS.IPF
.*This file contains ZED Keys help.
.*It is intended to be imbedded in ZED.IPF
.*Created : 02/22/1995 Bruce Shankle
.********************************************

:h1 res=653.Keys help
:p.ZED provides keyboard accelerators for most options.

:p.General window keys.
:table cols='10 20' rules=both frame=box.
:row.
:c.Alt+F5
:c.Restore - Restore window state after a maximize or minimize.
:row.
:c.Alt+F7
:c.Move - Move window.
:row.
:c.Alt+F8
:c.Size - Change window to be sized.
:row.
:c.Alt+F9
:c.Minimize - Minimize window.
:row.
:c.Alt+F10
:c.Maximize - Maximize window.
:row.
:c.Alt+F4
:c.Close - Close ZED.
:row.
:c.Ctrl+Esc
:c.Window list - Display active program list.
:etable.

:p.ZED file menu accelerator keys.
:table cols='10 20' rules=both frame=box.
:row.
:c.Ctrl+O
:c.Open - Open a file.  Display file open dialog window.
:row.
:c.Ctrl+I
:c.Import - Import a file. Display file import dialog window.
:row.
:c.F2
:c.Save - Save current text.
:row.
:c.F4
:c.Save and exit - Save current text and exit ZED.
:row.
:c.F3
:c.Exit - Exit ZED.
:row.
:c.
:etable.

:p.ZED edit menu accelerator keys.
:table cols='10 20' rules=both frame=box.
:row.
:c.Ctrl+Z
:c.Undo - Undo last text or color change operation.
:row.
:c.Ctrl+X
:c.Cut - Remove selected text and place in clipboard.
:row.
:c.Ctrl+C
:c.Copy - Copy selected text to clipboard.
:row.
:c.Ctrl+V
:c.Paste - Insert text from clipboard at current cursor location.
:row.
:c.Del
:c.Delete - Remove selected text.
:row.
:c.Ctrl+F
:c.Find - Activate find dialog window.
:row.
:c.F5
:c.Find next - Find next instance of search text.
:row.
:c.Ctrl+R
:c.Replace - Activate replace dialog window.
:etable.

:p.ZED settings menu accelerator keys.
:table cols='10 20' rules=both frame=box.
:row.
:c.Ctrl+A
:c.Auto-indent - Toggle autoindent.
:row.
:c.Ctrl+E
:c.Case-sensitive filenames - Toggle use of case-sensitive filenames.
:row.
:c.Ctrl+S
:c.Status Bar - Toggle status bar.
:row.
:c.Ctrl+W
:c.Word wrap - Toggle word wrap.
:etable.

:p.ZED miscellaneous keys.

:table cols='15 20' rules=both frame=box.
:row.
:c.F1
:c.Help - Activate help.
:row.
:c.Ctrl+Insert
:c.Copy - Copy selected text to clipboard.
:row.
:c.Shift+Insert
:c.Paste - Insert text from clipboard at current cursor location.
:row.
:c.Escape
:c.Escape - Pressing escape on a dialog window will close that window.
:etable.

:p.:hp2.Edit window keys:ehp2.

:p.
The behavior of the edit window, 
when typing, depends on 
whether it is in insert or 
overtype mode, and whether 
the selection is empty or 
not.

:p.
When a character is typed, 
it replaces the current 
selection.  If the selection 
is empty, the character is 
viewed as replacing nothing, 
so the character is 
effectively inserted into the 
text.  If one or more 
characters are selected, 
those characters are deleted 
from the text and replaced 
by the typed character. 

:p.
If the edit window is in insert 
mode, the cursor is moved to follow the
typed character. 

:p.
If the edit window is in overtype 
mode, the cursor is moved 
to immediately follow the 
newly typed character and
characters after it are over typed.

:p.
If the typing causes the 
cursor to go off the screen 
in any direction, the display 
is automatically scrolled.  If 
word-wrap is on, text 
continues on a new line, 
otherwise, the screen is 
scrolled horizontally. 

:p.
Scrolling of the text in the 
window is independent of 
cursor movement.  The 
cursor and selection remain 
unaltered at the same 
location within the text 
during all scrolling but the 
converse is not true. Any 
movement of the cursor 
causes auto-scrolling, if 
necessary, to ensure that 
the text location of the 
cursor is visible within the 
window. 

:p.
Return: Return (ASCII 
newline) causes a hard 
line-break, and the 
following text begins on a 
new line.  A line-break 
character is inserted in the 
text, which is drawn as a 
few pels of white-space 
(for selection purposes). 

:p.
Keystroke commands: For 
all the following keys, 
unless otherwise noted, the 
display is scrolled, if 
necessary, to keep the 
cursor point visible.  Where 
noted, the cursor setting 
behaves differently in insert 
mode than in overtype mode.  
This is subject to the 
boundary conditions noted 
above. 

:p.
:hp2.Del:ehp2.
.br
Causes the contents of the 
selection region to be 
deleted.  If the selection 
region contains no text, it 
causes the character to 
the right of the cursor to 
be deleted. 

:p.
:hp2.Shift+Del:ehp2.
.br
Causes the contents of the 
selection region to be cut 
to the clipboard. 

:p.
:hp2.Insert:ehp2.
.br
Toggles between insert 
and overtype mode. The 
edit window ignores the Insert 
key when it occurs 
without a modifier. 

:p.
:hp2.Shift+Ins:ehp2.
.br
Causes the contents of the 
clipboard to replace the 
selection region. 

:p.
:hp2.Ctrl+Ins:ehp2.
.br
Causes the selection 
region to be copied to the 
clipboard.  The selection 
region is not otherwise 
affected. 

:p.
:hp2.Backspace:ehp2.
.br
Functions similar to Del.  
If the selection is not 
empty, Backspace deletes 
the selection.  If the 
selection is empty, 
Backspace deletes the 
character to the left of the 
cursor point. If the edit window is 
in overtype mode, the 
anchor point is set, and 
the cursor point is moved 
to be one character 
previous in the text.  If 
no such character exists 
(because the anchor is set 
to the beginning of the 
text) the cursor is set to 
the anchor point.  If the 
edit window is in insert mode, 
the cursor and anchor 
points are set, as defined 
at the start of this 
chapter. 

:p.
:hp2.Down Arrow:ehp2.
.br
Sets the cursor point to 
the closest insertion point 
on the following line, then 
sets the anchor point to 
the cursor point (insertion 
mode) or one character 
following (overtype mode). 

:p.
:hp2.Shift+Down Arrow:ehp2.
.br
Causes the cursor point to 
be moved to the closest 
insertion point on the 
following line.  The 
anchor point does not 
move. 

:p.
:hp2.Up Arrow:ehp2.
.br
Sets the cursor point to 
the closest insertion point 
on the preceding line, then 
sets the anchor point to 
the cursor point (insert 
mode) or one character 
following (overtype mode). 

:p.
:hp2.Shift+Up:ehp2.
.br
Sets the cursor point to 
the closest insertion point 
on the preceding line.  
The anchor point is not 
moved. 

:p.
:hp2.Right Arrow:ehp2.
.br
Sets the cursor point to 
the insertion point one 
character following the 
cursor point.  The anchor 
point is set to the cursor 
point (insert mode) or one 
character following 
(overtype mode). 

:p.
:hp2.Shift+Right:ehp2.
.br
Causes the cursor point to 
be set to the insertion 
point immediately 
following the previous 
cursor point.  The anchor 
point is not moved. 

:p.
:hp2.Left and Shift+Left:ehp2.
.br
Work analogously. 

:p.
:hp2.Ctrl+Right:ehp2.
.br
Moves the cursor point to 
the insertion point 
immediately preceding the 
next word in the text 
including trailing spaces, 
and sets the anchor point 
to be equal to (insert 
mode) or one character 
following (overtype mode) 
the cursor point. The EOL 
(hard line-break) and tab 
characters are treated as 
words. 

:p.
:hp2.Ctrl+Shift+Right:ehp2.
.br
Moves only the cursor 
point in the same way as 
Ctrl+Right, but leaves the 
anchor point unmoved. 

:p.
:hp2.Ctrl+Left:ehp2.
.br
Moves the cursor point to 
the preceding insertion 
point at the beginning of a 
word, and sets the anchor 
point to be equal to 
(insert mode) or one 
character following 
(overtype mode) the cursor 
point. The EOL (hard 
line-break) and tab 
characters are treated as 
words. 

:p.
:hp2.Ctrl+Shift+Left:ehp2.
.br
Moves only the cursor 
point in the same way as 
Ctrl+Left but leaves the 
anchor point unmoved. 

:p.
:hp2.Pagedown and Pageup:ehp2.
.br
Cause the display to be 
scrolled one screen at a 
time in either direction.  
This behavior is the same 
as would be encountered 
during a page-down or 
page-up caused by the 
scroll-bar. 

:p.
:hp2.Ctrl+Pagedown and Ctrl+Pageup:ehp2.
.br
Cause the display to be 
scrolled one screen at a 
time to the right or left 
respectively. This 
behavior is the same as 
would be encountered 
during a page-right or 
page-left caused by the 
scroll-bar. 

:p.
:hp2.Home:ehp2.
.br
Sets the cursor point to 
the insertion point at the 
beginning of the line 
containing the cursor 
point, and sets the anchor 
point equal to (insert 
mode) or one character 
following (overtype mode). 

:p.
:hp2.Shift+Home:ehp2.
.br
Moves the cursor point to 
the insertion point at the 
beginning of the line.  The 
anchor point is not moved. 

:p.
:hp2.End:ehp2.
.br
Sets the anchor point to 
the insertion point at the 
end of the line containing 
the cursor point.  If the 
last character on the line 
is a line-break character, 
the anchor is positioned 
just before it.  The cursor 
is set equal to (insert 
mode) or one character 
previous to (overtype 
mode) the anchor. 

:p.
:hp2.Shift+End:ehp2.
.br
Moves the cursor point to 
the insertion point at the 
end of the line, as above.  
The anchor point is not 
moved. 

:p.
:hp2.Ctrl+Home:ehp2.
.br
Moves the cursor point to 
the insertion point at the 
beginning of the document.  
The anchor point is set 
equal to (insert mode) or 
one character following it 
(overtype mode). 

:p.
:hp2.Ctrl+End:ehp2.
.br
Moves the anchor point to 
the insertion point at the 
end of the document.  The 
cursor point is set to be 
equal to the anchor point 
(insert mode) or one 
character preceding it 
(overtype mode). 

:p.
:hp2.Ctrl+Shift+Home:ehp2.
.br
Moves the cursor point in 
the same way as 
Ctrl+Home, but leaves the 
anchor point unmoved. 

:p.
:hp2.Ctrl+Shift+End:ehp2.
.br
Moves the cursor point in 
the same way as 
Ctrl+End, but leaves the 
anchor point unmoved. 

.br


:i1.Keys

.*Link back to general help.
.br
:link reftype=hd res=657.General Help:elink.

.*End keys help
.********************************************
