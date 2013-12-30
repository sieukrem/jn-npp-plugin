// create MenuCmds Object in global scope
MenuCmds = new function(){
	this._IDM  = 40000;
	var self = this;
	
	var createCmdFn = function (cmdName, id){
		self["_"+cmdName] = id;
		self[cmdName] = function(plus){
			return Editor.runMenuCmd(id + (plus?plus:0));
		}
	};

	createCmdFn("FILE", (this._IDM + 1000));
	createCmdFn("FILE_NEW", (this._FILE + 1));
	createCmdFn("FILE_OPEN", (this._FILE + 2));
	createCmdFn("FILE_CLOSE", (this._FILE + 3));
	createCmdFn("FILE_CLOSEALL", (this._FILE + 4));
	createCmdFn("FILE_CLOSEALL_BUT_CURRENT", (this._FILE + 5));
	createCmdFn("FILE_SAVE", (this._FILE + 6));
	createCmdFn("FILE_SAVEALL", (this._FILE + 7));
	createCmdFn("FILE_SAVEAS", (this._FILE + 8));
	createCmdFn("FILE_ASIAN_LANG", (this._FILE + 9));
	createCmdFn("FILE_PRINT", (this._FILE + 10));
	createCmdFn("FILE_PRINTNOW", 1001);
	createCmdFn("FILE_EXIT", (this._FILE + 11));
	createCmdFn("FILE_LOADSESSION", (this._FILE + 12));
	createCmdFn("FILE_SAVESESSION", (this._FILE + 13));
	createCmdFn("FILE_RELOAD", (this._FILE + 14));
	createCmdFn("FILE_SAVECOPYAS", (this._FILE + 15));
	createCmdFn("FILE_DELETE", (this._FILE + 16));
	createCmdFn("FILE_RENAME", (this._FILE + 17));
	 
	createCmdFn("FILEMENU_LASTONE", this._FILE_RENAME);
	 
	createCmdFn("EDIT", (this._IDM + 2000));
	createCmdFn("EDIT_CUT", (this._EDIT + 1));
	createCmdFn("EDIT_COPY", (this._EDIT + 2));
	createCmdFn("EDIT_UNDO", (this._EDIT + 3));
	createCmdFn("EDIT_REDO", (this._EDIT + 4));
	createCmdFn("EDIT_PASTE", (this._EDIT + 5));
	createCmdFn("EDIT_DELETE", (this._EDIT + 6));
	createCmdFn("EDIT_SELECTALL", (this._EDIT + 7));
		
	createCmdFn("EDIT_INS_TAB", (this._EDIT + 8));
	createCmdFn("EDIT_RMV_TAB", (this._EDIT + 9));
	createCmdFn("EDIT_DUP_LINE", (this._EDIT + 10));
	createCmdFn("EDIT_TRANSPOSE_LINE", (this._EDIT + 11));
	createCmdFn("EDIT_SPLIT_LINES", (this._EDIT + 12));
	createCmdFn("EDIT_JOIN_LINES", (this._EDIT + 13));
	createCmdFn("EDIT_LINE_UP", (this._EDIT + 14));
	createCmdFn("EDIT_LINE_DOWN", (this._EDIT + 15));
	createCmdFn("EDIT_UPPERCASE", (this._EDIT + 16));
	createCmdFn("EDIT_LOWERCASE", (this._EDIT + 17));

	createCmdFn("EDIT_BLOCK_COMMENT", (this._EDIT + 22));
	createCmdFn("EDIT_STREAM_COMMENT", (this._EDIT + 23));
	createCmdFn("EDIT_TRIMTRAILING", (this._EDIT + 24));
		
	createCmdFn("EDIT_RTL", (this._EDIT+26));
	createCmdFn("EDIT_LTR", (this._EDIT+27));
	createCmdFn("EDIT_SETREADONLY", (this._EDIT+28));
	createCmdFn("EDIT_FULLPATHTOCLIP", (this._EDIT+29));
	createCmdFn("EDIT_FILENAMETOCLIP", (this._EDIT+30));
	createCmdFn("EDIT_CURRENTDIRTOCLIP", (this._EDIT+31));

	createCmdFn("EDIT_CLEARREADONLY", (this._EDIT+33));
	createCmdFn("EDIT_COLUMNMODE", (this._EDIT+34));
	createCmdFn("EDIT_BLOCK_COMMENT_SET", (this._EDIT+35));
	createCmdFn("EDIT_BLOCK_UNCOMMENT", (this._EDIT+36));

	createCmdFn("EDIT_AUTOCOMPLETE", (50000+0));
	createCmdFn("EDIT_AUTOCOMPLETE_CURRENTFILE", (50000+1));
	createCmdFn("EDIT_FUNCCALLTIP", (50000+2));
		
	//Belong to MENU FILE
	createCmdFn("OPEN_ALL_RECENT_FILE", (this._EDIT + 40));
	createCmdFn("CLEAN_RECENT_FILE_LIST", (this._EDIT + 41));
		
	createCmdFn("SEARCH", (this._IDM + 3000));

	createCmdFn("SEARCH_FIND", (this._SEARCH + 1));
	createCmdFn("SEARCH_FINDNEXT", (this._SEARCH + 2));
	createCmdFn("SEARCH_REPLACE", (this._SEARCH + 3));
	createCmdFn("SEARCH_GOTOLINE", (this._SEARCH + 4));
	createCmdFn("SEARCH_TOGGLE_BOOKMARK", (this._SEARCH + 5));
	createCmdFn("SEARCH_NEXT_BOOKMARK", (this._SEARCH + 6));
	createCmdFn("SEARCH_PREV_BOOKMARK", (this._SEARCH + 7));
	createCmdFn("SEARCH_CLEAR_BOOKMARKS", (this._SEARCH + 8));
	createCmdFn("SEARCH_GOTOMATCHINGBRACE", (this._SEARCH + 9));
	createCmdFn("SEARCH_FINDPREV", (this._SEARCH + 10));
	createCmdFn("SEARCH_FINDINCREMENT", (this._SEARCH + 11));
	createCmdFn("SEARCH_FINDINFILES", (this._SEARCH + 13));
	createCmdFn("SEARCH_VOLATILE_FINDNEXT", (this._SEARCH + 14));
	createCmdFn("SEARCH_VOLATILE_FINDPREV", (this._SEARCH + 15));
	createCmdFn("SEARCH_CUTMARKEDLINES", (this._SEARCH + 18));
	createCmdFn("SEARCH_COPYMARKEDLINES", (this._SEARCH + 19));
	createCmdFn("SEARCH_PASTEMARKEDLINES", (this._SEARCH + 20));
	createCmdFn("SEARCH_DELETEMARKEDLINES", (this._SEARCH + 21));
	createCmdFn("SEARCH_MARKALLEXT1", (this._SEARCH + 22));
	createCmdFn("SEARCH_UNMARKALLEXT1", (this._SEARCH + 23));
	createCmdFn("SEARCH_MARKALLEXT2", (this._SEARCH + 24));
	createCmdFn("SEARCH_UNMARKALLEXT2", (this._SEARCH + 25));
	createCmdFn("SEARCH_MARKALLEXT3", (this._SEARCH + 26));
	createCmdFn("SEARCH_UNMARKALLEXT3", (this._SEARCH + 27));
	createCmdFn("SEARCH_MARKALLEXT4", (this._SEARCH + 28));
	createCmdFn("SEARCH_UNMARKALLEXT4", (this._SEARCH + 29));
	createCmdFn("SEARCH_MARKALLEXT5", (this._SEARCH + 30));
	createCmdFn("SEARCH_UNMARKALLEXT5", (this._SEARCH + 31));
	createCmdFn("SEARCH_CLEARALLMARKS", (this._SEARCH + 32));

	createCmdFn("VIEW", (this._IDM + 4000));

	//#define	IDM_VIEW_TOOLBAR_HIDE			(IDM_VIEW + 1)
	createCmdFn("VIEW_TOOLBAR_REDUCE", (this._VIEW + 2));
	createCmdFn("VIEW_TOOLBAR_ENLARGE", (this._VIEW + 3));
	createCmdFn("VIEW_TOOLBAR_STANDARD", (this._VIEW + 4));
	createCmdFn("VIEW_REDUCETABBAR", (this._VIEW + 5));
	createCmdFn("VIEW_LOCKTABBAR", (this._VIEW + 6));
	createCmdFn("VIEW_DRAWTABBAR_TOPBAR", (this._VIEW + 7));
	createCmdFn("VIEW_DRAWTABBAR_INACIVETAB", (this._VIEW + 8));
	createCmdFn("VIEW_POSTIT", (this._VIEW + 9));
	createCmdFn("VIEW_TOGGLE_FOLDALL", (this._VIEW + 10));
	createCmdFn("VIEW_USER_DLG", (this._VIEW + 11));
	createCmdFn("VIEW_LINENUMBER", (this._VIEW + 12));
	createCmdFn("VIEW_SYMBOLMARGIN", (this._VIEW + 13));
	createCmdFn("VIEW_FOLDERMAGIN", (this._VIEW + 14));
	createCmdFn("VIEW_FOLDERMAGIN_SIMPLE", (this._VIEW + 15));
	createCmdFn("VIEW_FOLDERMAGIN_ARROW", (this._VIEW + 16));
	createCmdFn("VIEW_FOLDERMAGIN_CIRCLE", (this._VIEW + 17));
	createCmdFn("VIEW_FOLDERMAGIN_BOX", (this._VIEW + 18));
	createCmdFn("VIEW_ALL_CHARACTERS", (this._VIEW + 19));
	createCmdFn("VIEW_INDENT_GUIDE", (this._VIEW + 20));
	createCmdFn("VIEW_CURLINE_HILITING", (this._VIEW + 21));
	createCmdFn("VIEW_WRAP", (this._VIEW + 22));
	createCmdFn("VIEW_ZOOMIN", (this._VIEW + 23));
	createCmdFn("VIEW_ZOOMOUT", (this._VIEW + 24));
	createCmdFn("VIEW_TAB_SPACE", (this._VIEW + 25));
	createCmdFn("VIEW_EOL", (this._VIEW + 26));
	createCmdFn("VIEW_EDGELINE", (this._VIEW + 27));
	createCmdFn("VIEW_EDGEBACKGROUND", (this._VIEW + 28));
	createCmdFn("VIEW_TOGGLE_UNFOLDALL", (this._VIEW + 29));
	createCmdFn("VIEW_FOLD_CURRENT", (this._VIEW + 30));
	createCmdFn("VIEW_UNFOLD_CURRENT", (this._VIEW + 31));
	createCmdFn("VIEW_FULLSCREENTOGGLE", (this._VIEW + 32));
	createCmdFn("VIEW_ZOOMRESTORE", (this._VIEW + 33));
	createCmdFn("VIEW_ALWAYSONTOP", (this._VIEW + 34));
	createCmdFn("VIEW_SYNSCROLLV", (this._VIEW + 35));
	createCmdFn("VIEW_SYNSCROLLH", (this._VIEW + 36));
	createCmdFn("VIEW_EDGENONE", (this._VIEW + 37));
	createCmdFn("VIEW_DRAWTABBAR_CLOSEBOTTUN", (this._VIEW + 38));
	createCmdFn("VIEW_DRAWTABBAR_DBCLK2CLOSE", (this._VIEW + 39));
	createCmdFn("VIEW_REFRESHTABAR", (this._VIEW + 40));
	createCmdFn("VIEW_WRAP_SYMBOL", (this._VIEW + 41));
	createCmdFn("VIEW_HIDELINES", (this._VIEW + 42));
	createCmdFn("VIEW_DRAWTABBAR_VERTICAL", (this._VIEW + 43));
	createCmdFn("VIEW_DRAWTABBAR_MULTILINE", (this._VIEW + 44));
	createCmdFn("VIEW_DOCCHANGEMARGIN", (this._VIEW + 45));

	createCmdFn("VIEW_FOLD", (this._VIEW + 50));
	createCmdFn("VIEW_FOLD_1", (this._VIEW_FOLD + 1));
	createCmdFn("VIEW_FOLD_2", (this._VIEW_FOLD + 2));
	createCmdFn("VIEW_FOLD_3", (this._VIEW_FOLD + 3));
	createCmdFn("VIEW_FOLD_4", (this._VIEW_FOLD + 4));
	createCmdFn("VIEW_FOLD_5", (this._VIEW_FOLD + 5));
	createCmdFn("VIEW_FOLD_6", (this._VIEW_FOLD + 6));
	createCmdFn("VIEW_FOLD_7", (this._VIEW_FOLD + 7));
	createCmdFn("VIEW_FOLD_8", (this._VIEW_FOLD + 8));

	createCmdFn("VIEW_UNFOLD", (this._VIEW + 60));
	createCmdFn("VIEW_UNFOLD_1", (this._VIEW_UNFOLD + 1));
	createCmdFn("VIEW_UNFOLD_2", (this._VIEW_UNFOLD + 2));
	createCmdFn("VIEW_UNFOLD_3", (this._VIEW_UNFOLD + 3));
	createCmdFn("VIEW_UNFOLD_4", (this._VIEW_UNFOLD + 4));
	createCmdFn("VIEW_UNFOLD_5", (this._VIEW_UNFOLD + 5));
	createCmdFn("VIEW_UNFOLD_6", (this._VIEW_UNFOLD + 6));
	createCmdFn("VIEW_UNFOLD_7", (this._VIEW_UNFOLD + 7));
	createCmdFn("VIEW_UNFOLD_8", (this._VIEW_UNFOLD + 8));
		
	createCmdFn("VIEW_GOTO_ANOTHER_VIEW", 10001);
	createCmdFn("VIEW_CLONE_TO_ANOTHER_VIEW", 10002);
	createCmdFn("VIEW_GOTO_NEW_INSTANCE", 10003);
	createCmdFn("VIEW_LOAD_IN_NEW_INSTANCE", 10004);

	createCmdFn("VIEW_SWITCHTO_OTHER_VIEW", (this._VIEW + 72));
		
	createCmdFn("FORMAT", (this._IDM + 5000));
	createCmdFn("FORMAT_TODOS", (this._FORMAT + 1));
	createCmdFn("FORMAT_TOUNIX", (this._FORMAT + 2));
	createCmdFn("FORMAT_TOMAC", (this._FORMAT + 3));
	createCmdFn("FORMAT_ANSI", (this._FORMAT + 4));
	createCmdFn("FORMAT_UTF_8", (this._FORMAT + 5));
	createCmdFn("FORMAT_UCS_2BE", (this._FORMAT + 6));
	createCmdFn("FORMAT_UCS_2LE", (this._FORMAT + 7));
	createCmdFn("FORMAT_AS_UTF_8", (this._FORMAT + 8));
	createCmdFn("FORMAT_CONV2_ANSI", (this._FORMAT + 9));
	createCmdFn("FORMAT_CONV2_AS_UTF_8", (this._FORMAT + 10));
	createCmdFn("FORMAT_CONV2_UTF_8", (this._FORMAT + 11));
	createCmdFn("FORMAT_CONV2_UCS_2BE", (this._FORMAT + 12));
	createCmdFn("FORMAT_CONV2_UCS_2LE", (this._FORMAT + 13));
		
	createCmdFn("LANG", (this._IDM + 6000));
	createCmdFn("LANGSTYLE_CONFIG_DLG", (this._LANG + 1));
	createCmdFn("LANG_C", (this._LANG + 2));
	createCmdFn("LANG_CPP", (this._LANG + 3));
	createCmdFn("LANG_JAVA", (this._LANG + 4));
	createCmdFn("LANG_HTML", (this._LANG + 5));
	createCmdFn("LANG_XML", (this._LANG + 6));
	createCmdFn("LANG_JS", (this._LANG + 7));
	createCmdFn("LANG_PHP", (this._LANG + 8));
	createCmdFn("LANG_ASP", (this._LANG + 9));
	createCmdFn("LANG_CSS", (this._LANG + 10));
	createCmdFn("LANG_PASCAL", (this._LANG + 11));
	createCmdFn("LANG_PYTHON", (this._LANG + 12));
	createCmdFn("LANG_PERL", (this._LANG + 13));
	createCmdFn("LANG_OBJC", (this._LANG + 14));
	createCmdFn("LANG_ASCII", (this._LANG + 15));
	createCmdFn("LANG_TEXT", (this._LANG + 16));
	createCmdFn("LANG_RC", (this._LANG + 17));
	createCmdFn("LANG_MAKEFILE", (this._LANG + 18));
	createCmdFn("LANG_INI", (this._LANG + 19));
	createCmdFn("LANG_SQL", (this._LANG + 20));
	createCmdFn("LANG_VB", (this._LANG + 21));
	createCmdFn("LANG_BATCH", (this._LANG + 22));
	createCmdFn("LANG_CS", (this._LANG + 23));
	createCmdFn("LANG_LUA", (this._LANG + 24));
	createCmdFn("LANG_TEX", (this._LANG + 25));
	createCmdFn("LANG_FORTRAN", (this._LANG + 26));
	createCmdFn("LANG_SH", (this._LANG + 27));
	createCmdFn("LANG_FLASH", (this._LANG + 28));
	createCmdFn("LANG_NSIS", (this._LANG + 29));
	createCmdFn("LANG_TCL", (this._LANG + 30));
	createCmdFn("LANG_LISP", (this._LANG + 31));
	createCmdFn("LANG_SCHEME", (this._LANG + 32));
	createCmdFn("LANG_ASM", (this._LANG + 33));
	createCmdFn("LANG_DIFF", (this._LANG + 34));
	createCmdFn("LANG_PROPS", (this._LANG + 35));
	createCmdFn("LANG_PS", (this._LANG + 36));
	createCmdFn("LANG_RUBY", (this._LANG + 37));
	createCmdFn("LANG_SMALLTALK", (this._LANG + 38));
	createCmdFn("LANG_VHDL", (this._LANG + 39));
	createCmdFn("LANG_CAML", (this._LANG + 40));
	createCmdFn("LANG_KIX", (this._LANG + 41));
	createCmdFn("LANG_ADA", (this._LANG + 42));
	createCmdFn("LANG_VERILOG", (this._LANG + 43));
	createCmdFn("LANG_AU3", (this._LANG + 44));
	createCmdFn("LANG_MATLAB", (this._LANG + 45));
	createCmdFn("LANG_HASKELL", (this._LANG + 46));
	createCmdFn("LANG_INNO", (this._LANG + 47));
	createCmdFn("LANG_CMAKE", (this._LANG + 48));
	createCmdFn("LANG_YAML", (this._LANG + 49));
		
	createCmdFn("LANG_EXTERNAL", (this._LANG + 50));
	createCmdFn("LANG_EXTERNAL_LIMIT", (this._LANG + 79));

	createCmdFn("LANG_USER", (this._LANG + 80)); 
	/** To set up your user defined language use following call
		MenuCmds.LANG_USER(1); 
		where 1 position of your language in the list of UDLs
	*/
	createCmdFn("LANG_USER_LIMIT", (this._LANG + 110));
		
		
	createCmdFn("ABOUT", (this._IDM  + 7000));
	createCmdFn("HOMESWEETHOME", (this._ABOUT  + 1));
	createCmdFn("PROJECTPAGE", (this._ABOUT  + 2));
	createCmdFn("ONLINEHELP", (this._ABOUT  + 3));
	createCmdFn("FORUM", (this._ABOUT  + 4));
	createCmdFn("PLUGINSHOME", (this._ABOUT  + 5));
	createCmdFn("UPDATE_NPP", (this._ABOUT  + 6));
	createCmdFn("WIKIFAQ", (this._ABOUT  + 7));
	createCmdFn("HELP", (this._ABOUT  + 8));


	createCmdFn("SETTING", (this._IDM + 8000));
	createCmdFn("SETTING_TAB_SIZE", (this._SETTING + 1));
	createCmdFn("SETTING_TAB_REPLCESPACE", (this._SETTING + 2));
	createCmdFn("SETTING_HISTORY_SIZE", (this._SETTING + 3));
	createCmdFn("SETTING_EDGE_SIZE", (this._SETTING + 4));
	createCmdFn("SETTING_IMPORTPLUGIN", (this._SETTING + 5));
	createCmdFn("SETTING_IMPORTSTYLETHEMS", (this._SETTING + 6));

	createCmdFn("SETTING_TRAYICON", (this._SETTING + 8));
	createCmdFn("SETTING_SHORTCUT_MAPPER", (this._SETTING + 9));
	createCmdFn("SETTING_REMEMBER_LAST_SESSION", (this._SETTING + 10));
	createCmdFn("SETTING_PREFERECE", (this._SETTING + 11));

	createCmdFn("SETTING_AUTOCNBCHAR", (this._SETTING + 15));

	// Menu macro
	createCmdFn("MACRO_STARTRECORDINGMACRO", (this._EDIT + 18));
	createCmdFn("MACRO_STOPRECORDINGMACRO", (this._EDIT + 19));
	createCmdFn("MACRO_PLAYBACKRECORDEDMACRO", (this._EDIT + 21));
	createCmdFn("MACRO_SAVECURRENTMACRO", (this._EDIT + 25));
	createCmdFn("MACRO_RUNMULTIMACRODLG", (this._EDIT+32));
			
	createCmdFn("EXECUTE", (this._IDM + 9000));
}();
