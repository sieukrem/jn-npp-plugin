(function(){
	var incMenu = Editor.addMenu("Includes");

	var includeDir = Editor.nppDir+"\\Plugins\\jN\\includes";
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var incDirObj = fso.GetFolder(includeDir);
	var openF = function(){
		Editor.open(this.path);
	}
	if (incDirObj){
		var filesEnum = new Enumerator(incDirObj.files);
		for (; !filesEnum.atEnd(); filesEnum.moveNext()){
			var file = filesEnum.item().Path;
			if (/\.js/i.test(file)){
				incMenu.addItem({
					path:file,
					text:filesEnum.item().Name,
					cmd:openF
				});
			}
		}
	}
})();