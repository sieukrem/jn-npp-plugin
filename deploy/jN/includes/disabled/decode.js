(function(){
	var decodeMenu = Editor.addMenu("Decode");
	var decodeFn = function(){
		var decoded = decodeFrom(this.cp, Editor.currentView.text);
		Editor.runMenuCmd(40000+5000+5); // set UTF-8 for document
		Editor.currentView.text = decoded;
	};

	dS = GlobalSettings.get("decode.cp");
	var lastItem = {
		text:dS?dS.text:"Cyrillic (Windows)",
		cmd:decodeFn,
		cp:dS?dS.cp:1251
	};
	lastItem["menuItem"] =  decodeMenu.addItem(lastItem);
	decodeMenu.addSeparator();
	require("lib/CodePages.js");

	var codePageGroups = ["IBM","Unicode","ISCII","Cyrillic","Arabic", "Chinese","Japanese","Korean","Hebrew","Baltic","Turkish","Western","European","(DOS)","(Windows)","(Mac)"].map(function(gr){
		return {	
			menu : decodeMenu.addMenu(gr),
			regex : new RegExp(gr)
		};
	});

	var setCP = function(){
		lastItem["cp"] = this.cp;
		lastItem.menuItem.text = this.text;
		GlobalSettings.set("decode.cp",{
			cp:lastItem["cp"],
			text:this.text
		});
		lastItem.cmd();
	}

	var otherCodePagesGroup = decodeMenu.addMenu("Other");
	codePages.list.forEach(function(cp){
		var added = false;
		codePageGroups.forEach(function(cpg){
			if (cpg.regex.test(cp.friendlyName)){
				added =true;
				cpg.menu.addItem({
					cp:cp.id,
					text:cp.friendlyName,
					cmd:setCP
				});
			}
		});
		if (!added){
			otherCodePagesGroup.addItem({
					cp:cp.id,
					text:cp.friendlyName,
					cmd:setCP
			});
		}
	 });
})();