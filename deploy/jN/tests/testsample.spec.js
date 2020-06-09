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
		downloadFile("https://raw.githubusercontent.com/sieukrem/jn-plugins/3972cb6c78f92feb1387f33aaa21355cfc65be19/plugins.json", process.env["TEMP"]+"/jn-test-plugins.json")
	});
	it("fails if file not exists", function (){
		expectThrow(function(){
			downloadFile("https://raw.githubusercontent.com/sieukrem/jn-plugins/3972cb6c78f92feb1387f33aaa21355cfc65be19/pluginss.json", process.env["TEMP"]+"/jn-tests-plugins.json")
		});
	});
});

function expectThrow(cb){
	try{
		cb();
	}catch(e){
		return;
	}
	throw new Error("Expected throw, but was not");
}

function downloadFile(url, dest){
	var xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
	if (xmlHttp){
		xmlHttp.open('GET', url, false);
		
		var finishFn = function(){ throw new Error("Unknown error") };
		
		xmlHttp.onreadystatechange = function () {
			if (xmlHttp.readyState != 4)
				return;
			
			finishFn = function(){
				if (xmlHttp.status != 200)
					throw new Error("Unexpected status "+xmlHttp.status);
				
				var response = xmlHttp.responseBody;
				
				var stream = new ActiveXObject("ADODB.Stream");
				
				stream.Type = 1; // binary
				stream.Open();
				stream.Write(response);
				stream.SaveToFile(dest, 2); // overwrite
			}
		};
		
		xmlHttp.send(null);
		
		finishFn();
	}


}