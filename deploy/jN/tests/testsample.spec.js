require("describe");

describe("process", function (){
	describe("env", function(){
		it("exists", function(){
			if (!process.env)
				throw new Error("property missing");
		});
	});
	describe("cwd", function(){
		it("returns path", function(){
			if (process.cwd().length>0)
				return;
		});
	});
	
	describe("pid", function(){
		it("exits", function(){
			if (process.pid > 0)
				return;
		});
	});
})

describe("downloadFile", function (){
	it("works", function (){		
		DownloadFile("https://raw.githubusercontent.com/sieukrem/jn-plugins/3972cb6c78f92feb1387f33aaa21355cfc65be19/plugins.json")
	});
});

function DownloadFile(url){
	var xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
	if (xmlHttp){
		xmlHttp.open('GET', url, false);
		
		xmlHttp.onreadystatechange = function () {
			if (xmlHttp.readyState == 4) {
				var response = xmlHttp.responseBody;
				/*
				var stream = new ActiveXObject("ADODB.Stream");

				stream.Open(2);
				stream.Write(response);
				stream.SaveToFile("d:\\temp\\plugins.json");
				
				var props = []
				for(var i in response){
					props.push(i);
				}
				
				alert(props.join(", "));*/
				
				console.log(xmlHttp.responseText);
			}
		};
		xmlHttp.send(null);
	}


}