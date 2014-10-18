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

	if (fso.FileExists(file))
		file = file;
	else if (fso.FileExists(file + ".js"))
		file = file + ".js"
	else if (fso.FileExists(require.currentDir+"\\"+file))
		file = require.currentDir+"\\"+file;
	else if (fso.FileExists(require.currentDir+"\\"+file+".js"))
		file = require.currentDir+"\\"+file+".js";
	else
		throw "File does not exist: " + file;
	
	// use full path to script, to avoid multiple loads
	var fileObj = fso.GetFile(file);
	file = fileObj.Path;

	// file already loaded
	if (require.hash[file])
		return require.hash[file].exports;
	
	var script = readFile(file, "UTF-8");
	var oldDir = require.currentDir;
	var oldModule = typeof(module) != "undefined"? module : "undefined";
	try{
		require.currentDir = fileObj.ParentFolder.Path;
		module = {exports:{}};
		
		System.addScript(script, file);
		
		require.hash[file] = { exports: module.exports };
		
	}finally{
		require.currentDir = oldDir;
		delete module;
		
		if (oldModule != "undefined")
			module = oldModule;
	}
	
	return require.hash[file].exports;
}

require["hash"] = {};
require["currentDir"] = new ActiveXObject("Scripting.FileSystemObject").GetFile(System.scriptFullName).ParentFolder.Path;
require["currentModule"] = {exports:{}};	

require("lib/ECMA262.js");

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
			f = fso.OpenTextFile(file,2, true,-1); // 2 for writing, -1 UTF-16
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
					settings = JSON.parse(fso.OpenTextFile(file,1, false,-1).ReadAll()); // 1 for reading, -1 UTF-16
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
GlobalListener = new Listener(['SHUTDOWN','READONLYCHANGED','LANGCHANGED','BUFFERACTIVATED','FILESAVED','FILECLOSED','FILEOPENED','CHARADDED','DOUBLECLICK','CLICK','UPDATEUI','MODIFYATTEMPTRO','CONTEXTMENU']);
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