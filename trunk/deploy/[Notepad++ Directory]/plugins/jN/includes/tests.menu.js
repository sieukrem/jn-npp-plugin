var testMenu = Editor.addMenu("Tests");

/*
	Editor samples
*/
var editorMenu = testMenu.addMenu("Editor");


var htoolbar = editorMenu.addItem({
	text:"Hide Toolbar",
	cmd:function(){
		Editor.toolBarHidden  = !Editor.toolBarHidden;
		htoolbar.checked = Editor.toolBarHidden;
	}
});

var hmenu = editorMenu.addItem({
	text:"Hide Main Menu",
	cmd:function(){
		Editor.menuHidden  = !Editor.menuHidden;
		hmenu.checked = Editor.menuHidden;
	}
});

var htabbar = editorMenu.addItem({
	text:"Hide Tabbar",
	cmd:function(){
		Editor.tabBarHidden  = !Editor.tabBarHidden;
		htabbar.checked = Editor.tabBarHidden;
	}
});

editorMenu.addSeparator();

var mmenu = editorMenu.addItem({
	text:"Show Message",
	cmd:function(){
		Editor.alert("<Message> ;-)");
	}
});

editorMenu.addItem({
	text:"Languages",
	cmd:function(){
		alert(Editor.langs.join(", "));
	}
});

/*
	System
*/

var systemMenu = testMenu.addMenu("System");
systemMenu.addItem({
	text:"Show Clipboard Data",
	cmd:function(){
		alert("Clipboard:\n" + System.clipBoard);
	}

});

systemMenu.addItem({
	text:"Selection to Clipboard",
	cmd:function(){
		System.clipBoard = currentView.selection ;
	}

});

/*
	View
*/
var viewMenu = testMenu.addMenu("View");

viewMenu.addItem({
	text:"Current File",
	cmd:function(){
		var files = currentView.files;
		Editor.alert("File "+(Editor.currentView.file + 1)+" of "+ files.length +"\nPath: "+files[currentView.file]);
	}
});

viewMenu.addItem({
	text:"Prev File",
	cmd:function(){
		Editor.currentView.file--;
	}
});

viewMenu.addItem({
	text:"Next File",
	cmd:function(){
		Editor.currentView.file++;
	}
});

viewMenu.addItem({
	text:"Files",
	cmd:function(){
		Editor.alert( Editor.currentView.files.join(', '));
	}
});

viewMenu.addItem({
	text:"Language",
	cmd:function(){
		alert(Editor.langs[ currentView.lang]);
	}
});


/**
	Menu examples
*/

var menuSub = testMenu.addMenu("Menu");

var toggle = menuSub.addItem({
	text:"Toggle checked",
	cmd:function(){
		toggle.checked =!toggle.checked;
	}
});

var oninitpopup = menuSub.addMenu({
	text:"oninitpopup",
	counter: 0,
	oninitpopup:function(){
		this.counter++;
		oninitpopup.text = "oninitpopup "+this.counter;
	}
});

var disableMenu = menuSub.addMenu("Disable");
var d1 = disableMenu.addItem({
	text:"d1: enable d2",
	cmd:function(){
		d2.disabled = false;
		d1.disabled = true;
	}
});

var d2 = disableMenu.addItem({
	text:"d2: enable d1",
	cmd:function(){
		d1.disabled = false;
		d2.disabled = true;
	}
});

var d3 = disableMenu.addItem({
	text:"d3: disable Parent Menu",
	cmd:function(){
		disableMenu.disabled = true;
	}
});


/**
	Test for Internet Explorer based Dialog 
*/
testMenu.addSeparator();
var hmenu = testMenu.addItem({
	text:"Show Dialog",
	cmd:function(){
		var dialog = new Dialog({
			npp:Editor,
			html: "<input type='text' id='id-text' value='text text text'/><div onclick='Dialog.cfg.myOnClickHandler(document.getElementById(\"id-text\").value); Dialog.close();'>click me!</div>", 
			Height:200,
			Width:200,
			Top: 200,
			title: "Dialog test",
			myOnClickHandler:function(newtext){
				MenuCmds.FILE_NEW();
				currentView.text = newtext;
			}
		});

		dialog.show();
	}
});
testMenu.addItem({
	text:"Show Dockable",
	cmd:function(){
		var d = Editor.createDockable({onbeforeclose:function(){ alert ("bc")}});
		var d = d.document;
		
		d.write("<html><head></head><body></body></html>");
		d.close();
		d.body.innerHTML = "aa";
		delete d;
	}	
});


/**
	WMI Samples
*/
var wmi = testMenu.addMenu("WMI Samples");
wmi.addItem({
	text:"Show User Login",
	cmd:function(){
		var objWMIService = GetObject("winmgmts:{impersonationLevel=impersonate}!//./root/cimv2");
		var colLogFiles = new Enumerator(objWMIService.ExecQuery("Select * from Win32_ComputerSystem"));
		var user=colLogFiles.item().UserName;
		alert(user);
	}
});

/**
	Position Samples
*/
var pos = viewMenu.addMenu("Position Samples");
pos.addItem({
	text:"get pos",
	cmd:function(){
		alert(currentView.pos);
	}
});

pos.addItem({
	text:"set pos++",
	cmd:function(){
		currentView.pos++;
	}
});

pos.addItem({
	text:"get bytePos",
	cmd:function(){
		alert(currentView.bytePos);
	}
});

pos.addItem({
	text:"set bytePos++",
	cmd:function(){
		currentView.bytePos++;
	}
});

pos.addSeparator();

pos.addItem({
	text:"get anchor",
	cmd:function(){
		alert(currentView.anchor);
	}
});

pos.addItem({
	text:"set anchor++",
	cmd:function(){
		currentView.anchor++;
	}
});

pos.addItem({
	text:"get byteAnchor",
	cmd:function(){
		alert(currentView.byteAnchor);
	}
});

pos.addItem({
	text:"set byteAnchor++",
	cmd:function(){
		currentView.byteAnchor++;
	}
});

pos.addSeparator();

pos.addItem({
	text:"get column",
	cmd:function(){
		alert(currentView.column);
	}
});

/**
	Lines Samples
*/
var lines = viewMenu.addMenu("Lines Samples");
lines.addItem({
	text:"Lines",
	cmd:function(){
		alert( "Line "+(currentView.lines.current +1)+" of "+currentView.lines.count+"\n" + currentView.lines.get(currentView.lines.current).text);
	}
});
lines.addSeparator();
lines.addItem({
	text:"Count",
	cmd:function(){
		alert(currentView.lines.count);
	}
});
lines.addItem({
	text:"Current",
	cmd:function(){
		alert(currentView.lines.current+1);
	}
});


lines.addItem({
	text:"Current text",
	cmd:function(){
		var l = currentView.lines.get(currentView.lines.current); 
		alert(l.text);
	}
});
lines.addItem({
	text:"Change text of current",
	cmd:function(){
		var l = currentView.lines.get(currentView.lines.current); 
		l.text = ":-)";
	}
});
lines.addItem({
	text:"Current: start and end",
	cmd:function(){
		var l = currentView.lines.get(currentView.lines.current); 
		alert("Current: "+l.start+" " + l.end + "\n"+l.text);
	}
});

lines.addItem({
	text:"Current: byteStart and byteEnd",
	cmd:function(){
		var l = currentView.lines.get(currentView.lines.current); 
		alert("Current: "+l.byteStart+" " + l.byteEnd + "\n"+l.text);
	}
});
lines.addItem({
	text:"Current: +1",
	cmd:function(){
		currentView.lines.current++; 
	}
});
lines.addItem({
	text:"Current: -1",
	cmd:function(){
		currentView.lines.current--; 
	}
});

