(function(){
	var helperCC = null; 
	try{
		helperCC = new ActiveXObject("ClearCase.Application");
	}catch(e){
		// ClearCase is not installed
	}

	if (!helperCC)
		return;

	var shell = new ActiveXObject("WScript.Shell");
	if (!shell)
		return;
		
	function getFile(){
		return Editor.currentView.files[Editor.currentView.file];
	};

	var updateOnInitPopupFn = [];
	
	var ccMenu = Editor.addMenu({
		text:"ClearCase",
		get:function(name,file){
			try{
				return helperCC[name](file);
			}catch(ex){
				//alert(ex.message);
			}
			return null;			
		},
		oninitpopup:function(){
			var fileFullPath = getFile();
			var version = this.get("Version", fileFullPath);
			var view =    this.get("View", fileFullPath);
			//var ccDir  = this.getView(fileFullPath.replace(/\\[^\\]+$/, ""));
			
			for(var i=0; i<updateOnInitPopupFn.length; i++){
				updateOnInitPopupFn[i](fileFullPath, version, view);
			}
		}
	});

	
	var	openVersionTree = ccMenu.addItem({
		text:"Open Version Tree",
		cmd:function(){
			shell.run('clearvtree \"'+getFile()+'\"');
		}
	});
	var	openVersionHistory = ccMenu.addItem({
		text:"Open Version History",
		cmd:function(){
			shell.run('clearhistory \"'+getFile()+'\"');
		}
	});
	var	openVersionProperties = ccMenu.addItem({
		text:"Open Version Properties",
		cmd:function(){
			shell.run('cleardescribe \"'+getFile()+'\"');
		}
	});

	ccMenu.addSeparator();
	
	var	compareWithPreviousVersion = ccMenu.addItem({
		text:"Compare with previous Version",
		cmd:function(){
			shell.run('cleartool diff -g -pre \"'+getFile()+'\"', 0);
		}
	});
	
	var	openPreviousVersion = ccMenu.addItem({
		text:"Open Previous Version",
		getTempFile:function(file){
			var m = file.match(/\\([^\\]*)\.(.*)$/);
			var extension = m[2];
			var name = m[1];
			var timestamp = new Date().getTime();
			var tempFolder = shell.ExpandEnvironmentStrings("%Temp%");
			var tempFile = tempFolder + '\\'+ name +"_"+ timestamp + '.' + extension;
			
			return tempFile;
		},
		exists:function(file){
			var fso = new ActiveXObject("Scripting.FileSystemObject");
			return fso.FileExists(file);
		},
		cmd:function(){
			var file = Editor.currentView.files[Editor.currentView.file];
			var	version = helperCC.Version(file);
			var pred = version.Predecessor.ExtendedPath;
			if (!this.exists(pred)){
				var tmp = this.getTempFile(file);
				shell.run("cleartool get -to \"" + tmp + "\" \"" + pred + "\"", 0, true);
				pred = tmp;
			}
			open(pred);
			
			MenuCmds.VIEW_GOTO_ANOTHER_VIEW();
		}
	});
		
	ccMenu.addSeparator();
	
	var	checkOut = ccMenu.addItem({
		text:"Check Out...",
		cmd:function(){
			shell.run("cleardlg /window 2808d6 /windowmsg A065 /checkout \""+getFile()+'\"');
		}
	});
	
	var	undoCheckOut = ccMenu.addItem({
		text:"Undo Checkout...",
		cmd:function(){
			shell.run("cleardlg /window 2808d6 /windowmsg A065 /uncheckout \""+getFile()+'\"');
		}
	});
	
	var	checkIn = ccMenu.addItem({
		text:"Check In...",
		cmd:function(){
			shell.run("cleardlg /window 2808d6 /windowmsg A065 /checkin \""+getFile()+'\"');
		}
	});
	
	var	undoHijack = ccMenu.addItem({
		text:"Undo Hijack",
		cmd:function(){
			shell.run('cleartool update -force -rename \"'+getFile()+'\"', 0);
		}
	});
	
	ccMenu.addSeparator();
	var	addToSourceControl = ccMenu.addItem({
		text:"Add to Source Control...",
		cmd:function(){
			shell.run('cleardlg /window 510562 /windowmsg A065 /addtosrc \"'+getFile()+'\"');
		}
	});
	
	ccMenu.addSeparator();
	
	var	findCheckout = ccMenu.addItem({
		text:"Find checkouts...",
		cmd:function(){
			var fileFullPath = getFile();
			fileFullPath = fileFullPath.substring(0,fileFullPath.lastIndexOf("\\")) // current folder
			fileFullPath = fileFullPath.substring(0,fileFullPath.lastIndexOf("\\")) // up one level
			shell.run('clearfindco \"'+fileFullPath+'\"', 0);
		}
	});

	function uFn(m, fn){
		return function(file,version, view){ return fn(m, file, version, view); }
	}
	
	updateOnInitPopupFn.push(uFn(openVersionHistory,  function(m, file,version, view){m.disabled = !version;}));
	updateOnInitPopupFn.push(uFn(compareWithPreviousVersion,  function(m, file,version, view){m.disabled = !version || !version.Predecessor || version.isHijacked;}));
	updateOnInitPopupFn.push(uFn(addToSourceControl,  function(m, file,version, view){m.disabled = !view    || !!version;}));
	updateOnInitPopupFn.push(uFn(checkOut,            function(m, file,version, view){m.disabled = !version || version.IsCheckedOut;}));
	updateOnInitPopupFn.push(uFn(checkIn,             function(m, file,version, view){m.disabled = !version || !version.IsCheckedOut;}));
	updateOnInitPopupFn.push(uFn(undoCheckOut,        function(m, file,version, view){m.disabled = !version || !version.IsCheckedOut;}));
	updateOnInitPopupFn.push(uFn(undoHijack,          function(m, file,version, view){m.disabled = !version || !version.isHijacked;}));
	updateOnInitPopupFn.push(uFn(openVersionTree,     function(m, file,version, view){m.disabled = !version;}));
	updateOnInitPopupFn.push(uFn(openVersionProperties, function(m, file,version, view){m.disabled = !version;}));
	updateOnInitPopupFn.push(uFn(openPreviousVersion, function(m, file,version, view){m.disabled = !version || !version.Predecessor;}));
	updateOnInitPopupFn.push(uFn(findCheckout,        function(m, file,version, view){m.disabled = !view;}));
	
	// Add handler to catch event if user tries to modify checked in file
	// and allow him to check out it
	GlobalListener.addListener({ MODIFYATTEMPTRO:function(v){
		var file = getFile();
		var version = helperCC.Version(file);
		
		if (version == null || version.IsCheckedOut)// nothing to do from point of view of ClearCase
			return;
			
		shell.run("cleardlg /window 2808d6 /windowmsg A065 /checkout \""+file+'\"');
	}});
	
})();
