/**
 *	Please place your code in *.js files in includes folder
 */

/**

 	Use GlobalListener.addListener(yoursCfg); to add new event listener
	and  GlobalListener.removeListener(yoursCfg); to remove your event listener

	For example
	GlobalListener.addListener({
		// you can place in this config object any your data and methods, not event handler only
		myTextField : "tru ta ta la la la",
		myOwnMethod:function(eventname){
			var v = Editor.currentView;
			Editor.alert(eventname + '\n' + v.files[v.file]);
		},
		
		SHUTDOWN:function(){
			//this.myOwnMethod("shutdown");
		},
		BUFFERACTIVATED:function(v,pos){
			var files = v.files;
			this.myOwnMethod("bufferactivated "+files[pos]);
		},
		READONLYCHANGED:function(v,pos){
			this.myOwnMethod("readonly "+pos);
		},
		LANGCHANGED:function(v,pos){
			this.myOwnMethod("langchanged "+pos);
		},
		FILECLOSED:function(){
			//this.myOwnMethod("FILECLOSED");
		},
		FILEOPENED:function(v,pos){
			this.myOwnMethod("FILEOPENED "+ pos);
		},		
		FILESAVED:function(v,pos){
			this.myOwnMethod("FILESAVED "+pos);
		},
		CHARADDED:function(v, pos){
			//alert(v.selection);
		},
		DOUBLECLICK:function(v, pos){
			if (v.selection.length > 2){
				MenuCmds.SEARCH_UNMARKALLEXT1();
				MenuCmds.SEARCH_MARKALLEXT1();
			}
		}
	});
*/


var jN = {
	/**
		Copies fields of this to dest if it does not
		exist in dest. Returns dest.
	*/
	copyTo : function(dest,src){
		for(var f in src){
			if (typeof(dest[f]) != "undefined")
				continue;
				
			dest[f] = src[f];
		}
	},
	empty : function(){}
}

/**
	calls fn for each el in an Array. Returns an new array of results of fn calls
	XXX: had to make this function closer to ECMA-262 5th spec since original
	implementation breaks compatibility for Underscore library
*/
if (!Array.prototype.forEach) {
	Array.prototype.forEach = function(fn, scope) {
		var result = [];
		for (var i = 0, len = this.length; i < len; ++i) {
			result.push(fn.call(scope || this, this[i], i, this));
		}

		return result;
	};
}
/**
 * returns index of el in an Array, otherwise -1
 * XXX ECMA-262 implemetation
 */
if (!Array.prototype.indexOf) {
    Array.prototype.indexOf = function (searchElement /*, fromIndex */ ) {
        "use strict";
        if (this == null) {
            throw new TypeError();
        }
        var t = Object(this);
        var len = t.length >>> 0;
        if (len === 0) {
            return -1;
        }
        var n = 0;
        if (arguments.length > 0) {
            n = Number(arguments[1]);
            if (n != n) { // shortcut for verifying if it's NaN
                n = 0;
            } else if (n != 0 && n != Infinity && n != -Infinity) {
                n = (n > 0 || -1) * Math.floor(Math.abs(n));
            }
        }
        if (n >= len) {
            return -1;
        }
        var k = n >= 0 ? n : Math.max(len - Math.abs(n), 0);
        for (; k < len; k++) {
            if (k in t && t[k] === searchElement) {
                return k;
            }
        }
        return -1;
    };
}

/**
	for internal use
*/
function debug(e){
	Editor.alert(stringifyException(e));
}
/**
	for internal use
*/
function stringifyException(e){
	var result="";
	for(var i in e)
		result+=i+"="+e[i]+"\r\n";
		
	return result;
}

/**
	for internal use only, see GlobalListener
*/
function Listener(eventNames){
	var handler = {};
	var self = this;
	
	var handle = function(evN, args){
		var evHs = handler[evN];
		var result;
		if (evHs)
			for (var i=0,c=evHs.length; i<c; i++)
				try{
					result = evHs[i][evN].apply(evHs[i], args);
				}catch(e){
					debug(e);
				}
		return result;
	};
	
	eventNames.forEach(function(evN){
		// create for each event an array of handlers
		handler[evN] = [];
		// create for each event an function with the same name
		self[evN] = function(n){return function(){ return handle(n,arguments);};}(evN);
	});
	
	this.addListener = function(l){
		for(var evN in handler){
			if (l[evN]){
				if (handler[evN].indexOf(l)==-1)
					handler[evN].push(l);
			}
		}
	};
};

/**
	Define global JSON object if native one is not 
	available. Native JSON object has more stable 
	implementation.
*/
(function(){
	if (typeof(JSON) != "undefined")
		return;
		
	var rexQuote = /\"/g;
	var rexSlash = /\\/g;
	
	function direct(v){
		return v;
	};
	function unk2str(unk){
		var type = typeof(unk);
		return conv[(type == "object" && unk.join) ?"array":type](unk);
	};
	var	conv = {
		"boolean"	: direct,
		"string"	: function(str){	return  '"'+str.replace(rexSlash, '\\\\').replace(rexQuote, '\\"')+'"';},
		"number" 	: direct,
		"object" 	:function(obj){
						var r = "", comma = "";
						for(var el in obj){
							r+= comma+unk2str(el)+':'+unk2str(obj[el]);
							comma = ",";
						}
						return  "{"+r+"}";
					},
		"array"		:function(arr){ 
						var r="",comma="";
						for(var i=0, c=arr.length; i<c; i++){
							r+= comma+unk2str(arr[i]);
							comma = ",";
						}
						return "["+r+"]";
					},
		"undefined"	:direct,
		"function"	:direct
	};
	
	JSON = {
		stringify : function(value){
			return unk2str(value);
		},
		parse : function(str){
			return eval("(" + str + ")");
		}
	};
})();
	
/**
	Read all bytes of file with given charset
*/
function readFile(path, charset) {
	var stream = new ActiveXObject("ADODB.Stream");

	stream.Charset = charset;
	stream.Open();
	stream.LoadFromFile(path);

	var result = stream.ReadText();
	stream.close();

	return result;
}
	
/**
	Load script file once.
*/
function require(file){
	var fso = new ActiveXObject("Scripting.FileSystemObject");

	if (!fso.FileExists(file)){
		var startDir = fso.GetFile(System.scriptFullName).ParentFolder.Path;
		if (fso.FileExists(startDir+"\\"+file))
			file = startDir+"\\"+file;
		else
			throw "File does not exist: " + file;
	}
	
	// use full path to script, to avoid multiple loads 
	file = fso.GetFile(file).Path;

	// file already loaded
	if (require.hash[file])
		return;
	
	var script = readFile(file, "UTF-8");

	System.addScript(script, file);
		
	require.hash[file] = true;
}

require["hash"] = {};	

/**
	Is an Interface for Setting and Reading of Settings
	get(name);
	set(name,valueStr);
*/
function Settings(file){
	var settings = null;
	
	var save = function(){
		var fso = new ActiveXObject("Scripting.FileSystemObject");
		var f,e;
		try{
			f = fso.OpenTextFile(file,2, true,-1); // for writing ASCII
			f.Write(JSON.stringify(settings));
		}catch(e){
			debug(e);
		}
		if (f)
			f.Close();
	};
	
	this.get = function(name){
		var fso = new ActiveXObject("Scripting.FileSystemObject");
		if (settings == null){ // try to read
			if (fso.FileExists(file)){
				try{
					settings = JSON.parse(readFile(file,"UTF-16"));
				}catch(e){
					debug(e);
				}
			}
		}

		return settings? settings[name] : null;
	};
	
	this.set = function(name, valueStr){
		var result = this.get(name);

		if (!settings)
			settings = {};
		
		settings[name] = valueStr;
		save();
		
		return result;
	};
};


/**
* Checks if settings file exists in pluginConfigDir, if not then 
* tries to copy existing file from nppDir\Plugins\jN
*/
function DetectAndMoveSettings(){
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var newSettings = Editor.pluginConfigDir+"\\jN.settings.js";
	if (fso.FileExists(newSettings))
		return;
	
	// check if old settings file exists
	var oldSettings = Editor.nppDir+"\\Plugins\\jN\\settings.js";
	if (!fso.FileExists(oldSettings))
		return;
	
	var oldFile = fso.GetFile(oldSettings);
	
	oldFile.Copy(newSettings);
	
	alert(	"jN has detected old USER INdependent Settings file \n"
			+oldSettings+
			"\nand copied it to USER DEpendent\n"
			+ newSettings +
			"\nPlease keep in mind that at next time jN will use new location to load and save settings "+
			" created by javascript plugins."
			);
}

function LoadSettings(){
	//GlobalSettings = new Settings(Editor.nppDir+"\\Plugins\\jN\\settings.js");
	GlobalSettings = new Settings(Editor.pluginConfigDir+"\\jN.settings.js");
}

DetectAndMoveSettings();
LoadSettings();

// initialize Listener with known event names
GlobalListener = new Listener(['SHUTDOWN','READONLYCHANGED','LANGCHANGED','BUFFERACTIVATED','FILESAVED','FILECLOSED','FILEOPENED','CHARADDED','DOUBLECLICK','CLICK','UPDATEUI','MODIFYATTEMPTRO']);
Editor.setListener(GlobalListener);

var loadIdleHandler = {
	fso : new ActiveXObject("Scripting.FileSystemObject"),
	errors : [],
	/** 
		Reads and runs your JavaScript file.
		Use UTF-8 or UTF-8 with BOM for new include files!
	*/
	include : function (file){
		if (this.fso.FileExists(file)){
			try{
				addScript(readFile(file, "UTF-8"), file);
			}catch(e){
				this.errors.push(e);
			}
		}
	},	
	cmd:function(){
		var includeDir = Editor.nppDir+"\\Plugins\\jN\\includes";
		var incDirObj = this.fso.GetFolder(includeDir);
		if (incDirObj){
			var filesEnum = new Enumerator(incDirObj.files);
			for (; !filesEnum.atEnd(); filesEnum.moveNext()){
				var file = filesEnum.item().Path;
				if (/\.js$/i.test(file)){
					this.include(file);
				}
			}
		}
	},
	millis:1000
}
//loadIdleHandler.cmd();
//Editor.addIdleHandler(loadIdleHandler);
setTimeout(loadIdleHandler);