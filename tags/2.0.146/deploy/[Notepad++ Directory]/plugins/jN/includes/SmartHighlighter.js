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