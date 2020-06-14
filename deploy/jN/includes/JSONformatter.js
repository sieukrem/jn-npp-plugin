(function(){
	var jsonMenuCfg = {
		text:"JSON",
		oninitpopup:function(){
			// this.jsonMenu.disabled  = Editor.langs[currentView.lang] != "JSON";
		}
	};

	var jsonMenu = Editor.addMenu(jsonMenuCfg);

	jsonMenuCfg.jsonMenu = jsonMenu.addItem({
		text:"Format",
		cmd:function(){
			var formatted = JSON.stringify(JSON.parse(currentView.text), null, 2);
			currentView.text = formatted;
		}
	});
})();