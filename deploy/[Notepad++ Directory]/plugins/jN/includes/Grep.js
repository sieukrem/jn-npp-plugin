(function(){
	var grepMenu = Editor.addMenu("Grep");
	
	function grep(text, regexStr){
		var matches = [];
		var rx = new RegExp("(\r?\n+|\n+|\r+)|"+regexStr, "g");
		var m=null;
		
		var lineBegin = 0;
		var matched = false;
		
		function push(index, length){
			if(matched){
				matches.push({capture:text.substring(lineBegin, index), pos:lineBegin});
				matched = false;
			}
			lineBegin = index + length;
		}
		
		while(m=rx.exec(text)){
			if (m[1]!=null && m[1].length>0){
				push(m.index, m[1].length)
				continue;
			}
			matched = true;
		}
		
		push(text.length, 0)
		
		return matches;
	}
	function select(arr, prop){
		var newArr = [];
		for(var i=0; i<arr.length; i++){
			newArr.push(arr[i][prop])
		}
		return newArr;
	}
	function outputToOpositeView(text){
		var view = currentView;
		var file = view.file;
		
		MenuCmds.FILE_NEW();
		MenuCmds.VIEW_GOTO_ANOTHER_VIEW();
		MenuCmds.FORMAT_UTF_8();
		
		currentView.text = text;
		view.file = file;
	}
	function grepDialog(text){
		var lastGrepExpression = GlobalSettings.get("Grep");
			
		Dialog.prompt("Regular expression", (lastGrepExpression && lastGrepExpression.expression)?lastGrepExpression.expression:"", function(expression){
			var matches = grep(text, expression);
			outputToOpositeView(select(matches,"capture").join("\n"))
			
			GlobalSettings.set("Grep",{"expression": expression});
		});	
	}
	grepMenu.addItem({
		text:"From Current View ...",
		cmd:function(){
			grepDialog(currentView.text);
		}
	})
	grepMenu.addItem({
		text:"From Selection ...",
		cmd:function(){
			grepDialog(currentView.selection);
		}
	})
	grepMenu.addItem({
		text:"From Clipboard ...",
		cmd:function(){
			grepDialog(System.clipBoard);
		}
	})
})()