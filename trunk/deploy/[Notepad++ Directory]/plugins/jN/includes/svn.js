(function(){
	var shell = new ActiveXObject("WScript.Shell");
	// check if svnCOM is installed
	// on this place can not be created, because running in Idle Handler
	var svnCOM = false;
	try{
		shell.RegRead("HKCR\\SubWCRev.object\\");
		svnCOM = true;
	}catch(e){}
	
	if (svnCOM){
		var svnMenu = Editor.addMenu({text:"SVN", oninitpopup:function(){
			svnObjects.BUFFERACTIVATED(currentView);
		}});
		var svnObjects = {
			svnObj : null,
			svn		: function(path){
				if (!this.svnObj)
					this.svnObj = new ActiveXObject("SubWCRev.object");
				this.svnObj.GetWCInfo(path,1,1);
				return this.svnObj;
			},
			fso		: function(){
				return new ActiveXObject("Scripting.FileSystemObject");
			},
			showLog	: svnMenu.addItem({
				text:"Show Log",
				cmd:function(){
					svnObjects.runShell("TortoiseProc.exe /command:log /path:\""+svnObjects.getFile()+'\"');
				}
			}),
			sepNull:svnMenu.addSeparator(),
			runShell:function(cmd){
				var shell = new ActiveXObject("WScript.Shell");
				if (shell && cmd)
					shell.run(cmd);
			},
			getFile:function(){
				return Editor.currentView.files[Editor.currentView.file];
			},
			update:svnMenu.addItem({
				text:"Update...",
				cmd:function(){
					svnObjects.runShell("TortoiseProc.exe /command:update /closeonend:1 /path:\""+svnObjects.getFile()+'\" /closeonend:0');
				}
			}),
			revert:svnMenu.addItem({
				text:"Revert...",
				cmd:function(){
					svnObjects.runShell("TortoiseProc.exe /command:revert /closeonend:1 /path:\""+svnObjects.getFile()+'\" /closeonend:0');
				}
			}),
			commit:svnMenu.addItem({
				text:"Commit...",
				cmd:function(){
					svnObjects.runShell("TortoiseProc.exe /command:commit /closeonend:1 /path:\""+svnObjects.getFile()+'\" /closeonend:0');
					svnObjects.BUFFERACTIVATED(currentView);
				}
			}),
			sepNull:svnMenu.addSeparator(),
			addToSourceControl	: svnMenu.addItem({
				text:"Add to Source Control...",
				cmd:function(){
					svnObjects.runShell("TortoiseProc.exe /command:add /path:\""+svnObjects.getFile()+'\"');
					svnObjects.BUFFERACTIVATED(currentView);
				}
			}),
			timerCmd: function(){
				if ( svnObjects.timerCfg != this ){
					return; // do nothing because a new timer runs
				}
				
				svnObjects.timerCfg = null;
				
				var fso = svnObjects.fso();
		
				var svnItem = null;
				
				if (fso.FileExists(this.file))			
					svnItem = svnObjects.svn(this.file);
			
				svnObjects.commit.disabled = !svnItem || !svnItem.IsSvnItem || !svnItem.HasModifications;
				svnObjects.revert.disabled = !svnItem || !svnItem.IsSvnItem || !svnItem.HasModifications;
				svnObjects.update.disabled = !svnItem || !svnItem.IsSvnItem;
				svnObjects.showLog.disabled = !svnItem || !svnItem.IsSvnItem;

				svnObjects.addToSourceControl.disabled = !svnItem || svnItem.IsSvnItem;
			},
			timerCfg: null,
			// event handler
			BUFFERACTIVATED	:function(v){
				// setup the new timer
				this.timerCfg = {
					file: v.files[v.file],
					millis: 1000, // increase value if SVN ActiveX to slow 
					cmd: this.timerCmd
				}
				this.timerCfg.cmd();
				//setTimeout(this.timerCfg);
			},
			FILESAVED: function (){
				this.BUFFERACTIVATED(currentView);
			}
		};
		// do first check
		svnObjects.BUFFERACTIVATED(Editor.currentView); // disable this call if deadlock :-( 
		
		//GlobalListener.addListener(svnObjects); // disable event handler if to slow 
	}
}
)();
