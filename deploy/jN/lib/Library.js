function Library(path){
	this.NativeLibrary = System.loadLibrary(path);
}

var nativeSize = System.isX64()? "pushLWORD" : "pushDWORD";
/*
   In case of x64 system it is important to respect parameter passing through register
   There are integer and floating point register, hence we use appropriate serializer function
*/

Library.prototype.Types = {
	"HWND"    : nativeSize,
	"UINT"    : "pushDWORD",
	"WPARAM"  : nativeSize,
	"LPARAM"  : "pushPtr",
	"BYTE"    : "pushByte",
	"WORD"    : "pushWord",
	"DWORD"   : "pushDWORD",
	"LWORD"   : "pushLWORD",
	"BOOL"    : "pushDWORD",
	"LPCTSTR" : "pushPtr",
	"LPCSTR"  : "pushPtr", 
	"LPSTR"   : "pushPtr",
	"LPWSTR"  : "pushPtr",
	"LPCWSTR" : "pushPtr",
	"LONG_PTR": "pushPtr",
	"int"     : "pushDWORD",
	"POINT"	  : "pushLWORD",
	"LPPOINT" : "pushPtr",
	"LPBOOL"  : "pushPtr",
	"FLOAT"   : "pushFloat",
	"DOUBLE"  : "pushDouble"
};

Library.prototype.Define = function(){ // name, params types...
	var name = arguments[0];
	var types = Array.prototype.slice.call(arguments).slice(1); // create array from arguments object and then extract types part
	
	if (this[name])
		throw "Function definition exists for "+name;
	
	// check types
	for(var i=0,c=types.length; i<c; i++){
		if (!this.Types[types[i]])
			throw "Unknown type "+types[i];
	}
	
	this[name] = function(){
		if (arguments.length != types.length)
			throw "Expected count of arguments is " + types.length+" but found "+arguments.length;
			
		var callFrame = System.createCallFrame();
		
		for(var i=0, c=arguments.length; i<c; i++){
			var serializerFunction = this.Types[types[i]];
			callFrame[serializerFunction](arguments[i]);
		}
		
		return this.NativeLibrary.callWithFrame(name, callFrame);
	}
}
