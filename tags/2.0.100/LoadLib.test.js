var Types = {
	DWord  : 4,
	Word     : 2,
	BOOL     : 'DWord',
	BSTR     : 'DWord',
	DWORD  : 'DWord',
	HWND     : 'DWord',
	HANDLE:'DWord',
	HINSTANCE : 'DWord',
	LPCTSTR : 'BSTR',
	UINT     : 'DWord',
	LPPROC:'DWord',
	LPARAM :'DWord',
	'int'  : 'DWord',
	'void': 0,
	GetSimple:function(name){
		var ft = this[name];
		if (typeof(ft) == 'number')
			return name;
		
		if (ft == null)
			throw "Unknown type: '"+name+"'";
			
		return this.GetSimple(ft);
	},
	GetSize:function(name){
		var ft = this[this.GetSimple(name)];
		
		return ft;
	},
	Register:function(name, cfg){ // cfg = {name1:"type1", name2:"type2"}
		if (!this.Register.complex)
			this.Register.complex = {};
		
		if ( this[name])
			throw "Type exists: '"+name+"'";
			
		
		this.Register.complex[name] = cfg;
		this[name] = 'LPCTSTR';
	}
};

function Library(path){
	var lib = loadLibrary(path);
	this.lib  =lib;
	
	var BufSize = function (formals){
		var result = 0;
		for(var i=2, c=formals.length; i<c; i++){
			var ft  = Types.GetSize(formals[i]);
			result += ft;
		}
		return result;
	};
	
	var Push = function(mem,formals, actuals){
		var pos = 0;
		for(var i=2, c=formals.length; i<c; i++){ // begin with 2 because of 1. return value and 2. function name
			var type  = Types.GetSimple(formals[i]);  // translate custom type name to type name that undestands library object
			lib["write"+type](mem,pos,actuals[i-2]); // write bytes of actual parameter to the mem buffer accordingly to type
			pos += Types.GetSize(type);  // set next buffer position.
		}
	};
	
	this.Func = function(retType, name ){
		var formals = this.Func.arguments;
		var size  = BufSize(formals);
		this[name] = function(){
			var mem = lib.alloc(size);
			Push(mem,formals, this[name].arguments);
			return lib.call(name,mem);
		};
	};
};


var kernel32 = new Library("kernel32.dll");
kernel32.Func("void", "Sleep","DWORD");


Types.Register("OPENFILENAME",{ 
lStructSize	              :"DWORD", 
hwndOwner	              :"HWND", 
hInstance	              :"HINSTANCE", 
lpstrFilter	              :"LPCTSTR", 
lpstrCustomFilter :"LPTSTR", 
nMaxCustFilter	    :"DWORD", 
nFilterIndex	    :"DWORD", 
lpstrFile	              :"LPTSTR", 
nMaxFile	              :"DWORD", 
lpstrFileTitle	    :"LPTSTR", 
nMaxFileTitle	    :"DWORD", 
lpstrInitialDir	    :"LPCTSTR", 
lpstrTitle	              :"LPCTSTR", 
Flags	                         :"DWORD", 
nFileOffset	              :"WORD", 
nFileExtension	    :"WORD", 
lpstrDefExt	              :"LPCTSTR", 
lCustData	              :"LPARAM", 
lpfnHook	              :"LPPROC", 
lpTemplateName	    :"LPCTSTR"
});


var comdlg32 = new Library("comdlg32.dll");
comdlg32.Func("BOOL","GetOpenFileName","BOOL");



var user32 = new Library("user32.dll");
/*
var cbCfg = {
	windows:[], 
	cmd:function(mem){
		var hwnd  = lib.lib.readDWord(mem, 0);
		var param = lib.lib.readDWord(mem, 4);
		var buf = lib.lib.alloc(100);
		//alert("hwnd: " + hwnd + ", param" + param );
		var count = lib.GetWindowTextW(hwnd, buf, 100);
		if (count!=0){
			var str = lib.lib.alloc(count);
			lib.lib.copy(str,0,buf,0,count*2);
			this.windows.push(str.toString());
		
		}
		
		return true;
	}
};
var cb = System.registerCallBack(cbCfg,8);



try{
lib.Func("BOOL","EnumWindows","LPPROC","LPARAM");
lib.Func("int","GetWindowTextW","HWND","LPCTSTR","int");
*/
user32.Func("void","MessageBoxW","HWND", "LPCTSTR","LPCTSTR","UINT");

// user32.MessageBoxW(0,"ha","haha",0);
/*
lib.EnumWindows(cb,123);
alert(cbCfg.windows.length + cbCfg.windows.join(",\n"));
}catch(e){
	alert(e);
}*/
/*
var mem = lib.alloc(16);
lib.writeDWord(mem, 0, 1000);
//lib.writeDWord(mem, 4, 21);
lib.call("Sleep",mem);
*/