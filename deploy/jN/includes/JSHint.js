(function(){

function getJSHint(){
	if (typeof(JSHINT) != "undefined")
		return JSHINT;
	
	require("includes/JSHint/jshint.js");
	require("includes/JSHint/jshint.options.js");
	
	if (!JSHINT)
		throw "jshint.js not found";
	
	return JSHINT;
}

var settings = GlobalSettings.get("JSHint") || {passfail:true};

var JSHintMenuCfg = {
	text:"JSHint",
	oninitpopup:function(){
		this.verifyMenuItem.disabled  = !/\.js$/i.test(currentView.files[currentView.file]);
	}
};

var JSHintMenu = Editor.addMenu(JSHintMenuCfg);


JSHintMenuCfg.verifyMenuItem = JSHintMenu.addItem({
	text:"Verify",
	cmd:function(){
		var JSHint = getJSHint();
		
		var rules = JSHint.getRules();
		var allowed = {};
		for(var i=0; i<rules.length; i++){
			if (settings[rules[i].id] == 1)
				continue;
						
			allowed[rules[i].id] = true;
		}
		
		JSHint(currentView.text, allowed);
	    var results = JSHint.errors;
		var messages = [];
		var file = currentView.files[currentView.file];
		for(var i=0; i<results.length; i++){
			if (results[i] == null || results[i]["id"] == null)
				continue;
				
			messages.push(this.getCells(results[i], file));
		}
			
		this.showDialog(messages, file);	
	},
			
	getCells:function(m){
		return ["<i class='"+m.id.replace(/\(|\)/g, "")+"'/>",m.line,m.character,m.reason];
	},
	css: "i.warning{ background-position:-16px 0;} i.warning,i.error{width:16px; height:16px; background-repeat: no-repeat; background-image: URL(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAQCAYAAAB3AH1ZAAAAAXNSR0IArs4c6QAAAAZiS0dEAP8A/wD/oL2nkwAAAAlwSFlzAAALEwAACxMBAJqcGAAABOtJREFUSMe1lFtsVGUQx3/zfefs2XN2l25b1ra0dqEtLQ1CUVJRlAApjddQjGg0Jj6oiQmXqEFj4oPigxGiRAM8eHsxUSNCYor64IUYIyFAQwoFbexFKKYV2tKL2+6lu3s+HxbxQtEn53Fm/jO/b+bLwL/YJ044/pHtVPE/mlwr8Dro+1fefM7PpDIHTpyufx78/yp24t5gDFgPrAOagTgwAHQA3wAHl3+eHvmrxrpWsbk6/Mj87fOqSE9RdV/PA+Qz+/6teWdbsAXYGqlf2hZetpZgfCk6Wk5u4kLtzEBXbeLktw8lerraO9uCe25sTx/6Q6dnK7YTAq3LV3xXOm88gCSInpvTWvHr6K6vrzGFUxudFivk7Sq5adXqktZHcePL0G4IwaCDIQJlNbjxhWg/tSg7ebHpyXrT+9aP+bMAaraC86zYc7VPl4dH3+jAf/UU128vKqrU3ubZck8/7MSUzdY5DUuawstbEMsif6kHbQve4la0LeQu9SCWRXh5C3MaljQpm62nH3ZiswLsAG/FyqUvpt8+gNq8n8Rjn8KBo6y7uWrHDiT4z3wrYNaH43VtXvVCSA7jDx7GjB5DsmOFT5Ydg9Fj+IOHITmMV72QcLyuzQqY9bMCxJ2yN2uf8QJDJwwlz26k6NkNZD6D617KO9W298rVAKxzyioxk71IegiVHcBiBDNxqpCQ7MdiBJUdQNJDmMlenLJKrADrrgLYiRTf3rroCX9fB8b86R/LAC/0c1eDfupVpOivGu2YZjvkItP9qKkuLBlG62FU5ofCBNLdaD2MJcOoqS5kuh875KId03wVwEJ33sdV66flYl+aSKDgM0A6CeYCRNckdV3AfvdvE3BMXHsu2rPR/lmUdQllj0HuNGQnIXcGZY+hrHG0f7aQ57lYjon/DWAXUr/2jopW/6t+ihsjxGb+PBQxBThAV457brA3bofqKwBBBkTl0SEbFbFQ9hgS/A1xJ8kdr0Ws80jwN5Q9iopY6JCNqDxWkIErAK+BLCmK7Ys+lpLshI935BcsC0aiwkhUCJeBeMAQuK0z0mSpD66sIGA6MCl0eSNSMh+8HLgppPIurNtGUTWbwE2Bl0NK5qPLG8GkCro/ACJirV3zYOky83I3TuckRIAiiE4YiicM2BR8Cjie4+4bA6t2IrdeXsE3pM6jShuQaD0SroCQBufycAMCIY2EK5BoPaq0AVLnCzrA2g32koj1od3ZDeeABUCxC6kUdk8fRD2YAxRHIDMNQ4ZA6wzLuuT9lzNmkeVwkMSZdn+kqk3V34IJXIfY4xg6yQ2WAWVQugGTLYaZGH7PUVTiTLtyOAgg+7V6fEOl/55lA3OBuIYFHtTVwGBf4RUNi6H3HPSNQ28WbMjnhHeO6Ec2kftoeq+04BTtUjW3N+m6lYinwVagBfIGsj4mmSffdwT/58OnyExuC20xhwCsklBRLLttbzY9lUcXBxA7KBJ1DI4rUhMCX5ncVFKkIYGpSBvTnDL+eB4Z+8lUnthdQyZBaIs5lHpnchv9X2z1U7+0SWUjEosjXjEmOY4ZGcAMdsNQV7uCPe7l5gDysWK1Rh4QY5IWIhpjlEjhDogRDL4PykfMDJgcyEwh6iYNX2yCL4EgULJ6AXXP3MOdi6tpnhuh0Q1QnprhwmiC7h/O07F9P9+fvMgAMAxMALnfARodxAmP18qUAAAAAElFTkSuQmCC)}",
	header: ["","Line", "Col", "Description"],
	dockable:{name:"JSHint", docking:"bottom"},
	
	showDialog:function(messages, file){
		var dialCfg = new Dialog.Grid({
			view: Editor.currentView,

			dockable:this.dockable,
			rows:messages,
			title: file,
			header: this.header,
			css: this.css,
			onRowClick:function(cell, target){
				var row = cell.parentNode;
				var line = row.cells[1].innerText;
				var col  = row.cells[2].innerText;
				
				var lnNr = 1*line -1;
				
				// scroll to the line
				this.view.line = lnNr;
				
				//set current column
				var lineObj = this.view.lines.get(lnNr);
				
				var posNr = lineObj.start + col*1 -1;
				this.view.pos = posNr;
				this.view.anchor = posNr;
				
				// aktivate view
				MenuCmds.VIEW_SWITCHTO_OTHER_VIEW();
				if (this.view != Editor.currentView)
					MenuCmds.VIEW_SWITCHTO_OTHER_VIEW();
			}
		});
		
		new Dialog(dialCfg);
	}
})

JSHintMenu.addSeparator();


JSHintMenu.addItem({
	text:"Settings",
	cmd:function(){
	    var rules = getJSHint().getRules();
		var rows = [];
		for(var i=0,c=rules.length; i<c; i++){
			rows.push(this.getCells(rules[i]));
		}

		this.showDialog(rows);	
	},
	getCells:function(rule){
		var checked = settings[rule.id] != 1?"checked=''":""; // negative logic, if exists then not selected
		return ["<input type='checkbox' "+checked+" name='"+rule.id+"'>",rule.name,rule.desc];
	},

	showDialog:function(rules){
		var dialCfg = new Dialog.Grid({
			title : "JSHint - Settings",
			rows  : rules,
			header: ["","Title","Description"],
			clientHeight: 200,
			clientWidth: 400,
			onRowClick:function(cell, target){
				var row = cell.parentNode;

				if (target.tagName != "INPUT"){
					target = row.cells[0].firstChild;
					this.toggle(target);
				}
				
				if (!target.checked)
					settings[target.getAttribute("name")] = 1;
				else
					delete settings[target.getAttribute("name")];
				
				GlobalSettings.set("JSHint",settings);
			},
			toggle:function(checkbox){
				checkbox.checked = !checkbox.checked;
			}
		});
		
		new Dialog(dialCfg);
	}
})

JSHintMenu.addSeparator();

JSHintMenu.addItem({
	text:"Visit JSHint",
	cmd: function(){
		var shell = new ActiveXObject("WScript.Shell");
		if (shell){
			shell.run("http://jshint.com/");
		}
	}
});

})();