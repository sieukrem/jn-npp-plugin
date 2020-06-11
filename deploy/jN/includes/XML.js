(function(){

	
	var xmlMenuCfg = {
		text:"XML",
		oninitpopup:function(){
			xpathMenuItem.disabled = !Editor.langs[currentView.lang] || (Editor.langs[currentView.lang] && Editor.langs[currentView.lang].toLowerCase() != "xml"); 
		}
	}
	
	var xmlMenu = Editor.addMenu(xmlMenuCfg);
	
	var selectCfg = {
		text	: "Select XPath ...",
		select	: function(xpath){
			var xmlDoc = new ActiveXObject("MSXML2.DOMDocument.6.0");
			xmlDoc.validateOnParse = false;

			xmlDoc.setProperty("SelectionNamespaces", "xmlns:xsl='http://www.w3.org/1999/XSL/Transform'");
			
			
			try{
				xmlDoc.loadXML(currentView.text);
				var nodes = xmlDoc.selectNodes(xpath);
				var xmltext = "";
				
				if (!nodes || nodes.length == 0){
					alert("Selection returns an empty node set");
					return false;
				}
					
				
				// instrument selected elements with dummy attribute
				for(var i=0,c=nodes.length; i<c; i++){
					var n = nodes[i];
					var nodeType = n.nodeTypeString;
					
					if (nodeType == "element"){
						xmltext += n.xml+"\n";
						n.setAttribute("xml-sel","");
					} else
					if (nodeType == "attribute"){
						xmltext += n.text+"\n";
					} else
					if (nodeType == "text"){
						xmltext += n.text+"\n";
					} else
					if (nodeType == "cdatasection"){
						xmltext += n.text+"\n";
					}else{
						alert("unhandled "+n.nodeTypeString);
					}
				}
				
				var view = Editor.currentView;
				var file = view.file;
				
				MenuCmds.FILE_NEW();
				MenuCmds.VIEW_GOTO_ANOTHER_VIEW();
				
				Editor.currentView.text = xmltext;
				Editor.currentView.lang = Editor.langs.indexOf("XML");
				
				view.file = file;
				
				GlobalSettings.set("XML",{"xpath": xpath});
				return true;
			}catch(e){
				debug(e);
				return false;
			}
		},
		cmd		: function(){
			var self = this;
			var xml_settings = GlobalSettings.get("XML");
				
			Dialog.prompt("XPath", (xml_settings && xml_settings.xpath) ? xml_settings.xpath : "", function(value){
				return self.select(value);
			});
		}
	};
	
	var xpathMenuItem = xmlMenu.addItem(selectCfg);

})();