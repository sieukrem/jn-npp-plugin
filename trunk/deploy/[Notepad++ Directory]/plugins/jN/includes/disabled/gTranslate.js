(function(){
	var gTranslate = Editor.addMenu("gTranslate");

	var translate = function (){
		if (Editor.currentView.selection.length > 0){
			if (!this.xmlHttp){
				this.xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
			}
			var xmlHttp = this.xmlHttp;
			if (xmlHttp){
				xmlHttp.open('GET', 'http://ajax.googleapis.com/ajax/services/language/translate?v=1.0&q='+encodeURIComponent(Editor.currentView.selection)+'&langpair='+this.langPair, true);
				xmlHttp.onreadystatechange = function () {
					if (xmlHttp.readyState == 4 && xmlHttp.responseText) {
						try{
							var tr = eval("("+xmlHttp.responseText+")").responseData.translatedText;
							
							switch(GetMode()){
								case "show": alert(tr); break;
								case "replace":currentView.selection = tr; break;
								case "clipboard": clipBoard = tr; break;
							}
								
						}catch(e){
							alert("Error");
						}
					}
				};
				xmlHttp.send(null);
			}
		} else {
			alert("Nothing to translate");
		}
	}

	gS = GlobalSettings.get("gTranslate.lang");

	var lastItem = {
		text:(gS?gS.text:"Englisch - Deutsch")+"\tF8",
		cmd:translate,
		key:119,
		langPair:gS?gS.langPair:"en|de"
	};

	lastItem["menuItem"] =  gTranslate.addItem(lastItem);
	addHotKey(lastItem);

	var setLang = function (){
		lastItem["langPair"] = this.langSrc[0]+"|"+this.langDest[0];
		lastItem.menuItem.text = this.langSrc[1]+" - "+this.langDest[1] + "\tF8";
		GlobalSettings.set("gTranslate.lang",{
			langPair:lastItem["langPair"],
			text:this.langSrc[1]+" - "+this.langDest[1]
		});
		lastItem.cmd();
	}

	gTranslate.addSeparator();

	var modes = {};
	
	var toggleMode = function (m){
		SetMode(this.mode);
	}
	
	var SetMode = function(mode){
		GlobalSettings.set("gTranslate.mode",  mode);
		
		for(var el in modes)
			modes[el].m.checked = (el == mode);
	}
	
	var GetMode = function(){
		var mode = GlobalSettings.get("gTranslate.mode");
		
		if (!mode || mode=="")
			 mode = "show";	
			 
		return mode;
	}
	
	var CreateMenu = function(text, mode){
		var cfg  = { "text" : text, cmd:toggleMode, "mode": mode};
		modes[mode] = cfg;
		cfg["m"] = gTranslate.addItem(cfg);
	}

	CreateMenu("Show","show");
	CreateMenu("Replace Selection","replace");
	CreateMenu("Copy to clipboard","clipboard");

	
	SetMode(GetMode());
	
	gTranslate.addSeparator();

	var languages = [
		["af","Afrikaans"],
		["sq","Albanisch"],
		["ar","Arabisch"],
		["bg","Bulgarisch"],
		["zh-CN","Chinesisch"],
		["da","Dänisch"],
		["de","Deutsch"],
		["en","Englisch"],
		["et","Estnisch"],
		["fi","Finnisch"],
		["fr","Französisch"],
		["gl","Galicisch"],
		["el","Griechisch"],
		["iw","Hebräisch"],
		["hi","Hindi"],
		["id","Indonesisch"],
		["ga","Irisch"],
		["is","Isländisch"],
		["it","Italienisch"],
		["ja","Japanisch"],
		["yi","Jiddisch"],
		["ca","Katalanisch"],
		["ko","Koreanisch"],
		["hr","Kroatisch"],
		["lv","Lettisch"],
		["lt","Litauisch"],
		["ms","Malaysisch"],
		["mt","Maltesisch"],
		["mk","Mazedonisch"],
		["nl","Niederländisch"],
		["no","Norwegisch"],
		["pl","Polnisch"],
		["pt","Portugiesisch"],
		["ro","Rumänisch"],
		["ru","Russisch"],
		["sv","Schwedisch"],
		["sr","Serbisch"],
		["sk","Slowakisch"],
		["sl","Slowenisch"],
		["es","Spanisch"],
		["sw","Suaheli"],
		["tl","Tagalog"],
		["th","Thailändisch"],
		["cs","Tschechisch"],
		["tr","Türkisch"],
		["uk","Ukrainisch"],
		["hu","Ungarisch"],
		["vi","Vietnamesisch"],
		["cy","Walisisch"],
		["be","Weissrussisch"]
	];

	for(var i=0, c=languages.length; i<c; i++){
		var lang = languages[i];
		var lM = gTranslate.addMenu(lang[1]);
		for(var j=0, k=languages.length; j<k; j++){
			if (j!=i){
				var lJ = languages[j];
				var lI = lM.addItem({
					text:lJ[1],
					langSrc: lang,
					langDest:lJ,
					cmd:setLang
				});
			}
		}
	}
})();