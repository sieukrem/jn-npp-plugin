function Library(path){
	this.NativeLibrary = System.loadLibrary(path);
}
Library.prototype.Types = {
	"HWND"  :4,
	"UINT"  :4,
	"WPARAM":4,
	"LPARAM":4,
	"LPCTSTR" : 4, 
	"LONG_PTR": 4,
	"int"     : 4,
	"POINT"	  : 8,
	"LPPOINT" : 4,
	SizeToSimple:function(size){
		switch(size){
			case 1:return "Byte";
			case 2:return "Word";
			case 4:return "DWord";
			case 8:return "Int64";
		}
		throw "Unknown size "+size;
	}
};

Library.prototype.Define = function(){ // name, params types...
	var name = arguments[0];
	var types = Array.prototype.slice.call(arguments).slice(1); // create array from arguments object and then extract types part
	
	if (this[name])
		throw "Function definition exists for "+name;
	
	// check types
	var stackSize = 0;
	for(var i=0,c=types.length; i<c; i++){
		if (!this.Types[types[i]])
			throw "Unknown type "+types[i];
		
		stackSize += this.Types[types[i]];
	}
	
	this[name] = function(){
		if (arguments.length != types.length)
			throw "Expected count of arguments is " + types.length+" but found "+arguments.length;
			
		var stack = this.NativeLibrary.alloc(stackSize);
		var pos = 0;
		for(var i=0, c=arguments.length; i<c; i++){
			var size = this.Types[types[i]];
			this.NativeLibrary["write"+this.Types.SizeToSimple(size)](stack, pos, arguments[i]);
			pos+=size;
		}
		
		return this.NativeLibrary.call(name, stack);
	}
}
