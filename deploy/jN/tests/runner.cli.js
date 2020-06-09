

var fso = new ActiveXObject("Scripting.FileSystemObject");
/**
	Load script file once.
*/
function require(file){

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
		
		execScript(script, file);
		
		require.hash[file] = { exports: module.exports };
		
	}finally{
		require.currentDir = oldDir;
		delete module;
		
		if (oldModule != "undefined")
			module = oldModule;
	}
	
	return require.hash[file].exports;
}

var shell = new ActiveXObject("WScript.Shell");

require["hash"] = {};
require["currentDir"] = shell.CurrentDirectory;
require["currentModule"] = {exports:{}};

function execScript(script, filepath){
	try{
		eval(script)
	}catch(e){
		console.log("Error thrown in "+filepath);
		console.log(formatError(e));
	}
}

function formatError(e){
	var str = "";
	for(var i in e)
		str += i + ": " +e[i]+"\n";
	
	return str;
}

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

var console = {
		log: function(msg){
			WScript.Echo(msg);
		}
}

require("../lib/Nodejs")


function main(){
	console.log("Test Runner");
	
	var filesEnum = new Enumerator(fso.GetFolder(process.cwd()).files);
	for (; !filesEnum.atEnd(); filesEnum.moveNext()){
		var file = filesEnum.item().Path;
		if (/\.spec\.js$/i.test(file)){
			require(file);
		}
	}
	
	printResults(runTests());
}

function printResults(results, _path){
	if (!_path)
		_path = [];
	
	var path = results.name ? _path.concat([results.name]) : [];
	
	if (!!results.result){
		console.log("["+results.result+"]  "+path.join(" "));
		return;
	}
	
	for(var i=0; i< results.tests.length; i++){
		printResults(results.tests[i], path);
	}
}

main();
