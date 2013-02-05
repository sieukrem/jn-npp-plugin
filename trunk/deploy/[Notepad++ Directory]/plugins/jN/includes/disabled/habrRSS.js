var xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
if (xmlHttp){
	var habrMenu = Editor.addMenu("Хабрахабр");
	xmlHttp.open('GET', 'http://habrahabr.ru/rss/1c9d5ed4f0953ded81510256ed34c9be/', true);
	xmlHttp.onreadystatechange = function () {
		if (xmlHttp.readyState == 4 && xmlHttp.responseXML) {
			var titles = xmlHttp.responseXML.selectNodes("/rss/channel/item/title");
			for(var i=0, c=titles.length; i<c; i++){
				habrMenu.addItem({
					text: titles[i].text, 
					link: titles[i].selectSingleNode("../link").text, 
					cmd:function(){
						var shell = new ActiveXObject("WScript.Shell");
						if (shell && this.link){
							shell.run(this.link);
						}
				}});
			}
		}
	};
	xmlHttp.send(null);
}