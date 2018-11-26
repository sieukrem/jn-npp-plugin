(function(){

var settings = GlobalSettings.get("SmartHighlighter")||{mask:0xffffffff};

function getIndicatorMask(){
	return firstView.navBarIndicatorMask | secondView.navBarIndicatorMask;
}

function setIndicatorMask(value){
	firstView.navBarIndicatorMask = value;
	secondView.navBarIndicatorMask = value;
}

setIndicatorMask(settings.mask);

var smMenu = Editor.addMenu("Smart Highlighter");
smMenu.addItem({
	text:"Settings",
	cmd:function(m){
		var rows = [];
		var mask = getIndicatorMask();
		
		for(var i=0; i<32; i++){
			rows.push(this.getCells((mask & (1<<i)) != 0, i));
		}

		this.showDialog(rows);	
	},
	getCells:function(isOn, indicatorNr){
		var checked = isOn ? "checked=''" : "";
		return ["<input type='checkbox' "+checked+" name='"+indicatorNr+"'>",indicatorNr + 1];
	},

	showDialog:function(rules){
		var dialCfg = new Dialog.Grid({
			title : "Smart Highlighter - Settings",
			rows  : rules,
			header: ["","Indicator Number"],
			clientHeight: 200,
			clientWidth: 400,
			onRowClick:function(cell, target){
				var row = cell.parentNode;

				if (target.tagName != "INPUT"){
					target = row.cells[0].firstChild;
					this.toggle(target);
				}
				
				var mask = getIndicatorMask();
				var indicatorNr = target.getAttribute("name")*1;
				if (!target.checked)
					setIndicatorMask(mask & (~(1<<indicatorNr)));
				else
					setIndicatorMask(mask | (1<<indicatorNr));
				settings.mask = getIndicatorMask();
				GlobalSettings.set("SmartHighlighter", settings);
			},
			toggle:function(checkbox){
				checkbox.checked = !checkbox.checked;
			}
		});
		
		new Dialog(dialCfg);
	}
});

GlobalListener.addListener({
	lastHighlighted : null,
	uui:1,
	linesLimit: 80000, // play with this value if you have performance problems
	HideNavBar:function(v){
		// because of bad performance we hide navigation bar if too much lines
		var xml = (Editor.langs[v.lang]=='XML')? 4 : 1; // reduce linesLimit in case of xml file
		var hide = v.lines.count > (this.linesLimit / xml);
		v.navBarHidden = hide;
	},
	UPDATEUI:function(v){
		this.HideNavBar(v);

		if (v.navBarHidden)
			return;
		
		if (this.lastHighlighted!=null && this.lastHighlighted != v.bytePos){
			MenuCmds.SEARCH_UNMARKALLEXT5();
		}
	},
	BUFFERACTIVATED:function(v,pos){
		this.HideNavBar(v);
	},
	DOUBLECLICK:function(v, pos){
		if (v.navBarHidden)
			return;
			
		if (this.lastHighlighted != null){
			MenuCmds.SEARCH_UNMARKALLEXT5();
			this.lastHighlighted = null;
		}
		
		var sel = v.selection; // cache property access
		
		if (sel.length > 2){
			if (this.lastHighlighted != v.bytePos){
				MenuCmds.SEARCH_MARKALLEXT5();
				this.lastHighlighted = v.bytePos;
			}else{
				this.lastHighlighted = null;
			}
		}
	}
});

})();