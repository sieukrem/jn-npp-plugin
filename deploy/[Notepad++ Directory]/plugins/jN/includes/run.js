(function(){
	var runMenu = Editor.addMenu("Run");

	var rview = {
		text:"Current View\tF5",
		key: 116,
		//ctrl:true,
		cmd: function(){
			addScript(Editor.currentView.text);
		}
	};
	runMenu.addItem(rview);
	addHotKey(rview);

	var rsel = {
		text:"Selection\tShift+F5",
		shift:true,
		key:116,
		cmd: function(){
			addScript(Editor.currentView.selection);
		}
	};
	addHotKey(rsel);
	runMenu.addItem(rsel);

	var rclip = {
		text:"Clipboard\tCtrl+F5",
		ctrl:true,
		key:116,	
		cmd: function(){
			addScript(System.clipBoard);
		}
	};
	runMenu.addItem(rclip);
	addHotKey(rclip);
})();