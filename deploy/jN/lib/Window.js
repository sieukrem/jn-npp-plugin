require("User32.dll.js");

function Window(handle){
	this.Handle = handle;
}

Window.prototype.GetWindowText = function(){
	var bufferLength = User32.GetWindowTextLengthW(this.Handle);
	var buffer = User32.NativeLibrary.alloc(bufferLength*2 + 2);
	var copiedChars = User32.GetWindowTextW(this.Handle, buffer, buffer.length/2);
	return buffer;
}

Window.prototype.SetWindowText = function(text){
	User32.SetWindowTextW(this.Handle, text);
}

Window.FindByClass = function(parent, windowClass){
	if (parent == null)
		return null;
	
	var windowHandle = User32.FindWindowExW(parent, 0, windowClass, "");
	if (windowHandle == null)
		return null;
	
	return new Window(windowHandle); 
}