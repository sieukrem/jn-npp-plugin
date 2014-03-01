'jN' is the new name of the plugin. I decided to rename it because of interference with NppScripting plugin based on Lua. 

===Attention====
If you have old Version of this plugin (NppScripting), please remove it (NppScripting.dll and NppScripting) from 'Notepad++\plugins'.
Before you do this save your own scripts.

==Installation==

1. Copy contents of [Notepad++ Directory] into Notepad++ 'plugins' folder (basically, it's C:\Program Files\Notepad++\plugins\)
2. Restart Notepad++

"jN\includes" Folder contains several JavaScript Files. They implement some functionality.

zen coding.js - extends Npp with "Zen Coding" functionality,
decode.js - adds menu, that allows to decode text from some encoding to UTF-8,
clearcase.js - adds menu, that allows to work with files under clearcase version control,
gTranslate.js - adds menu, that allows you to translate some selected text,
MenuCmds.js - allows you to execute some standard menu actions of Npp,
run.js - adds menu and hotkey to run some javascript direkt from Npp,
Dialog.js - adds Internet Explorer based dialog functionality,
test.menu.js - some tests and samples of functionality of nppscripting plugin,
includes.js - adds new Menu, that allows you to open files in includes.

Move any of these files into "includes/disabled" to disable it. For example clearcase.js if you don't know what is this :-).