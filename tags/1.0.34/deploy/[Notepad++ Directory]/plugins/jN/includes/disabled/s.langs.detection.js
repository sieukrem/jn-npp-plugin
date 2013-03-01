var langsDetection = {
	langs:{
		"Newsfile":{regex:/(START|FINISH)/g,udlpos:1}
	},
/*	MENUCOMMAND:function(cmdId){
		alert(cmdId);
	},
*/	FILEOPENED:function(v, pos){
		// because opened file is not selected now
		v.file = v.files.length-1;
		var t = v.text;
		if(v.lang == 0)
			for(var ln in this.langs){
				var lo = this.langs[ln];
				if (t.match(lo.regex)){
					
					MenuCmds.LANG_USER(lo.udlpos);
					break;
				}
			}
	}
};

// check for opened files
for(var i=0,fs=currentView.files.length; i<fs; i++){
	langsDetection.FILEOPENED(currentView, i);
}

GlobalListener.addListener(langsDetection);

// var d = createDialog({}); alert(d.height); d.height += d.height; alert(d.height);