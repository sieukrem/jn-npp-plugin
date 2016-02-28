require("User32.dll.js");

/**
 * Creates new Editor wrapper
 */
function EditorJS(){
	this.editor = Editor;
}

EditorJS.prototype.Call = function(msg, lparam, wparam){
	return User32.SendMessageW(this.editor.handle, this.Const[msg], lparam, wparam);
}

EditorJS.prototype.GetLanguageName = function(languageId){
	var len = this.Call("NPPM_GETLANGUAGEDESC", languageId, 0);
	if (len<1)
		throw "Wrong language type id";
		
	var buf = User32.NativeLibrary.alloc(len + 1);
	User32.NativeLibrary.writeByte(buf, len, 0);
	
	var copied = this.Call("NPPM_GETLANGUAGEDESC", languageId, buf);
	
	return buf;
}

EditorJS.NPPMSG = 0x0400 + 1000; // WM_USER + 1000

EditorJS.prototype.Const = {
	NPPM_GETLANGUAGEDESC: EditorJS.NPPMSG + 84
};