(function(){
	var helperCC = null; 
	try{
		helperCC = new ActiveXObject("ClearCase.Application");
	}catch(e){
		// ClearCase is not installed
	}

	if (helperCC){
		var ccMenu = Editor.addMenu("ClearCase");
		var ccObjects = {
			openVersionTree	: ccMenu.addItem({
				text:"Open Version Tree",
				cmd:function(){
					var fileFullPath = Editor.currentView.files[Editor.currentView.file];
					var shell = new ActiveXObject("WScript.Shell");
					if (shell && fileFullPath){
						shell.run('clearvtree \"'+fileFullPath+'\"');
					}		
				}
			}),
			openVersionProperties	: ccMenu.addItem({
				text:"Open Version Properties",
				cmd:function(){
					var fileFullPath = Editor.currentView.files[Editor.currentView.file];
					var shell = new ActiveXObject("WScript.Shell");
					if (shell && fileFullPath){
						shell.run('cleardescribe \"'+fileFullPath+'\"');
					}		
				}
			}),
			openPreviousVersion: ccMenu.addItem({
				text:"Open Previous Version",
				cmd:function(){
					var file = Editor.currentView.files[Editor.currentView.file];
					var version;
					try{
						version = ccObjects.helperCC.Version(file);
						version = version.Predecessor;
						open(version.ExtendedPath);
						MenuCmds.VIEW_GOTO_ANOTHER_VIEW();
					}catch(e){
						debug(e);
					}				
					
				}
			}),
			sepNull:ccMenu.addSeparator(),
			runShell:function(cmd){
				var shell = new ActiveXObject("WScript.Shell");
				if (shell && cmd)
					shell.run(cmd);
			},
			getFile:function(){
				return Editor.currentView.files[Editor.currentView.file];
			},
			checkOut:ccMenu.addItem({
				text:"Check Out...",
				cmd:function(){
					ccObjects.runShell("cleardlg /window 2808d6 /windowmsg A065 /checkout \""+ccObjects.getFile()+'\"');
				}
			}),
			undoCheckOut:ccMenu.addItem({
				text:"Undo Checkout...",
				cmd:function(){
					ccObjects.runShell("cleardlg /window 2808d6 /windowmsg A065 /uncheckout \""+ccObjects.getFile()+'\"');
				}
			}),
			checkIn:ccMenu.addItem({
				text:"Check In...",
				cmd:function(){
					ccObjects.runShell("cleardlg /window 2808d6 /windowmsg A065 /checkin \""+ccObjects.getFile()+'\"');
				}
			}),
			sepNull:ccMenu.addSeparator(),
			addToSourceControl	: ccMenu.addItem({
				text:"Add to Source Control...",
				cmd:function(){
					ccObjects.runShell('cleardlg /window 510562 /windowmsg A065 /addtosrc \"'+ccObjects.getFile()+'\"');
				}
			}),
			helperFSO	: new ActiveXObject("Scripting.FileSystemObject"),
			helperCC	: helperCC,
			
			getView	:function(file){
				try{
					return this.helperCC.View(file);//e.item().ShareName);
				}catch(ex){
					// this.msg(ex.description);
				}
				return false;
			},
			msgON	: false,// ccSM.checked,
			msg		: function(msg){
				if (this.msgON)
					alert(msg);
			},
			// event handler
			READONLYCHANGED :function(){
				this.BUFFERACTIVATED(currentView);
			},
			BUFFERACTIVATED	:function(v){
				var file = v.files[v.file];
				var ccView = this.getView(file);
				var version = null;
				var predecessor = null;
				ccMenu.disabled = !ccView;
				if(ccView){
					try{
						version = this.helperCC.Version(file);
						predecessor = version.Predecessor;
					}catch(e){
						//debug(e);
					}

					this.addToSourceControl.disabled = !ccView || (ccView!=null && version!=null);
					
					this.undoCheckOut.disabled = (!version || !version.IsCheckedOut);
					this.checkOut.disabled = (!version || version.IsCheckedOut);
					this.checkIn.disabled = (!version || !version.IsCheckedOut);
					
					this.openVersionTree.disabled = !version;
					this.openVersionProperties.disabled = !version;
					this.openPreviousVersion.disabled = !predecessor;
				}else{
					return;
				}
			}
		};
		// do first check
		ccObjects.BUFFERACTIVATED(Editor.currentView);

		GlobalListener.addListener(ccObjects);
	}
})();