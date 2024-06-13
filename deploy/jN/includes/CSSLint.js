(function(){

function createDialog(cfg){
	var header = "";
	for(var i=0; i<cfg.header.length; i++)
		header +="<th>"+cfg.header[i]+"</th>";
	
	new Dialog({
		npp: Editor,
		html: "<table id='results' onMouseOver='Dialog.cfg.onMOver(window.event);' onMouseOut='Dialog.cfg.onMOut(window.event);' onclick='Dialog.cfg.onClick(window.event, Dialog);' onkeypress='Dialog.cfg.onKeyDown(window.event, Dialog);'><tr>"+header+"</tr></table>", 
		height: 300,
		width: 500,
		title: cfg.title,
		css: "body{overflow:auto;} .highlight{background-color:#eee;} #results{ font-size:12px; border-collapse:collapse} td{cursor:pointer;} td,th{border: 1px solid #d8d8d8; padding: 2px 10px} th{ background-color: #d8d8d8; padding-top:5px; text-align: center; vertical-align: middle;} i.warning{ background-position:-16px 0;} i.warning,i.error{width:16px; height:16px; background-repeat: no-repeat; background-image: URL(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAQCAYAAAB3AH1ZAAAAAXNSR0IArs4c6QAAAAZiS0dEAP8A/wD/oL2nkwAAAAlwSFlzAAALEwAACxMBAJqcGAAABOtJREFUSMe1lFtsVGUQx3/zfefs2XN2l25b1ra0dqEtLQ1CUVJRlAApjddQjGg0Jj6oiQmXqEFj4oPigxGiRAM8eHsxUSNCYor64IUYIyFAQwoFbexFKKYV2tKL2+6lu3s+HxbxQtEn53Fm/jO/b+bLwL/YJ044/pHtVPE/mlwr8Dro+1fefM7PpDIHTpyufx78/yp24t5gDFgPrAOagTgwAHQA3wAHl3+eHvmrxrpWsbk6/Mj87fOqSE9RdV/PA+Qz+/6teWdbsAXYGqlf2hZetpZgfCk6Wk5u4kLtzEBXbeLktw8lerraO9uCe25sTx/6Q6dnK7YTAq3LV3xXOm88gCSInpvTWvHr6K6vrzGFUxudFivk7Sq5adXqktZHcePL0G4IwaCDIQJlNbjxhWg/tSg7ebHpyXrT+9aP+bMAaraC86zYc7VPl4dH3+jAf/UU128vKqrU3ubZck8/7MSUzdY5DUuawstbEMsif6kHbQve4la0LeQu9SCWRXh5C3MaljQpm62nH3ZiswLsAG/FyqUvpt8+gNq8n8Rjn8KBo6y7uWrHDiT4z3wrYNaH43VtXvVCSA7jDx7GjB5DsmOFT5Ydg9Fj+IOHITmMV72QcLyuzQqY9bMCxJ2yN2uf8QJDJwwlz26k6NkNZD6D617KO9W298rVAKxzyioxk71IegiVHcBiBDNxqpCQ7MdiBJUdQNJDmMlenLJKrADrrgLYiRTf3rroCX9fB8b86R/LAC/0c1eDfupVpOivGu2YZjvkItP9qKkuLBlG62FU5ofCBNLdaD2MJcOoqS5kuh875KId03wVwEJ33sdV66flYl+aSKDgM0A6CeYCRNckdV3AfvdvE3BMXHsu2rPR/lmUdQllj0HuNGQnIXcGZY+hrHG0f7aQ57lYjon/DWAXUr/2jopW/6t+ihsjxGb+PBQxBThAV457brA3bofqKwBBBkTl0SEbFbFQ9hgS/A1xJ8kdr0Ws80jwN5Q9iopY6JCNqDxWkIErAK+BLCmK7Ys+lpLshI935BcsC0aiwkhUCJeBeMAQuK0z0mSpD66sIGA6MCl0eSNSMh+8HLgppPIurNtGUTWbwE2Bl0NK5qPLG8GkCro/ACJirV3zYOky83I3TuckRIAiiE4YiicM2BR8Cjie4+4bA6t2IrdeXsE3pM6jShuQaD0SroCQBufycAMCIY2EK5BoPaq0AVLnCzrA2g32koj1od3ZDeeABUCxC6kUdk8fRD2YAxRHIDMNQ4ZA6wzLuuT9lzNmkeVwkMSZdn+kqk3V34IJXIfY4xg6yQ2WAWVQugGTLYaZGH7PUVTiTLtyOAgg+7V6fEOl/55lA3OBuIYFHtTVwGBf4RUNi6H3HPSNQ28WbMjnhHeO6Ec2kftoeq+04BTtUjW3N+m6lYinwVagBfIGsj4mmSffdwT/58OnyExuC20xhwCsklBRLLttbzY9lUcXBxA7KBJ1DI4rUhMCX5ncVFKkIYGpSBvTnDL+eB4Z+8lUnthdQyZBaIs5lHpnchv9X2z1U7+0SWUjEosjXjEmOY4ZGcAMdsNQV7uCPe7l5gDysWK1Rh4QY5IWIhpjlEjhDogRDL4PykfMDJgcyEwh6iYNX2yCL4EgULJ6AXXP3MOdi6tpnhuh0Q1QnprhwmiC7h/O07F9P9+fvMgAMAxMALnfARodxAmP18qUAAAAAElFTkSuQmCC)}",
		
		oncreate: function(){
			var el = this.getElementById("results");
			
			var log="";
			for (i=0, len=cfg.arr.length; i < len; i++){
				var row = el.insertRow();
				this.Dialog.cfg.cell(row, cfg.getCells( cfg.arr[i] ));
			}

		},
		cell:function(r, arr){
			for(var i=0,c=arr.length; i<c; i++)
				r.insertCell().innerHTML = arr[i];
		},
		parent:function(el,name){
			if (el.tagName == name)
				return el;
			return this.parent(el.parentNode, name);
		},
		onMOver:function(evt){
			var target = evt.srcElement || evt.target;
			var row = this.parent(target, "TR");
			row.className = "highlight";
		},
		onMOut:function(evt){
			var target = evt.srcElement || evt.target;
			var row = this.parent(target, "TR");
			row.className = "";
		},
		onClick:function(evt, d){				
			if (typeof(cfg.onRowClick)!="function")
				return;
				
			var target = evt.srcElement || evt.target;
			var row = this.parent(target, "TR");
			
			cfg.onRowClick(row, target);
		},
		onKeyDown: function(evt, dialog) {
			var target = evt.srcElement || evt.target,
				keycode = evt.keyCode || evt.which;
				
			if (keycode == 27 || keycode == 13) { 
				dialog.hide();
			}
		}
	});
}

function getCSSLint(){
	if (typeof(CSSLint) != "undefined")
		return CSSLint;
	
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	
	var fn = Editor.nppDir + "\\plugins\\jN\\jN\\includes\\csslint\\csslint.js";
	if (fso.FileExists(fn)){
		var f = fso.OpenTextFile(fn,1, false,0);
		var scr = decodeFrom(65001,f.ReadAll());
		if (scr && scr.length>0){
			try{
				addScript(scr);
			}catch(e){
				alert(e.message);
			}
		}
		f.Close();
	}else{
		alert("csslint not found");
	}
	
	return CSSLint;
}

var settings = GlobalSettings.get("CSSLint") || {};

var CSSLintMenuCfg = {
	text:"CSS Lint",
	oninitpopup:function(){
		this.verifyMenuItem.disabled  = !/\.(css|php|html)/i.test(currentView.files[currentView.file]);
	}
	
};

var CSSLintMenu = Editor.addMenu(CSSLintMenuCfg);


CSSLintMenuCfg.verifyMenuItem = CSSLintMenu.addItem({
	text:"Verify",
	cmd:function(){
		var csslint = getCSSLint();
		var rules = csslint.getRules();
		var allowed = {};
		for(var i=0; i<rules.length; i++){
			if (settings[rules[i].id] == 1)
				continue;
						
			allowed[rules[i].id] = 1;
		}
		
	    var results = csslint.verify(currentView.selection||currentView.text, allowed);
		var messages = results.messages;
		//alert(JSON.stringify(messages));
		this.showDialog(messages);	
	},
	
	showDialog:function(messages){
		createGridViewDialog({
			arr:messages,
			title:"CSSLint - Results",
			header: ["","Line", "Col", "Title","Description"],
			getCells:function(m){
				return ["<i class='"+m.type+"'/>",m.line,m.col,m.rule.name,m.message];
			},
			
			onRowClick:function(row){
				var line = row.cells[1].innerText;
				var col = row.cells[2].innerText;
				Editor.currentView.line = 1*line -1;
				//this.npp.currentView.column = 1*col -1;

			}
		});
	}
})

CSSLintMenu.addSeparator();


CSSLintMenu.addItem({
	text:"Settings",
	cmd:function(){
	    var rules = getCSSLint().getRules();
		this.showDialog(rules);	
	},
	showDialog:function(rules){
		createGridViewDialog({
			arr:rules,
			title:"CSSLint - Settings",
			header: ["","Title","Description"],
			getCells:function(rule){
				var checked = settings[rule.id] != 1?"checked=''":""; // negative logic, if exists then not selected
				return ["<input type='checkbox' "+checked+" name='"+rule.id+"'>",rule.name,rule.desc];
			},
			
			onRowClick:function(row, target){
				var line = row.cells[1].innerText;
				if (target.tagName != "INPUT"){
					target = row.cells[0].firstChild;
					this.toggle(target);
				}
				
				if (!target.checked)
					settings[target.getAttribute("name")] = 1;
				else
					delete settings[target.getAttribute("name")];
				
				GlobalSettings.set("CSSLint",settings);
			},
			toggle:function(checkbox){
				checkbox.checked = !checkbox.checked;
			}
		});
	}
})

CSSLintMenu.addSeparator();

CSSLintMenu.addItem({
	text:"Visit CSS Lint",
	cmd: function(){
		var shell = new ActiveXObject("WScript.Shell");
		if (shell){
			shell.run("http://csslint.net");
		}
	}
});

})();
