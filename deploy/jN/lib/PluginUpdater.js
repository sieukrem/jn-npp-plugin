require("fs");

(function (){

module.exports.PluginUpdater =
function (registryPath, localPath){
    var plugins = null;

    this.getPluginsInTheRegistry = function (){
        if (!plugins)
            plugins = JSON.parse(fetchAsText(registryPath));

        return plugins;
    }

    this.updatePlugin = function (name){
        var plugins = this.getPluginsInTheRegistry();
        var plugin = plugins[name];
        if (!plugin)
            throw new Error("Plugin not found");

        var tmpDir = createTempDir();
        downloadPlugin(plugin.url, tmpDir);
        fs.renameSync(tmpDir, localPath+"\\"+name);
    }

    function createTempDir(){
        var path = fs.mkdtempSync(process.env["TEMP"]+"\\");
        fs.mkdirSync(path);
        return path;
    }

    function downloadPlugin(packageUrl, destDir){
        var packageJson = fetch(packageUrl, asText());
        var packageObj = JSON.parse(packageJson);

        var files = Array.isArray(packageObj.files)?packageObj.files:[];
        var packagePath = packageUrl.replace(/[^\/]+$/, "");

        for(var i=0; i<files.length; i++){
            var destFile = destDir+"\\"+files[i];
            fs.mkdirSync(destFile.replace(/[\\\/][^\\\/]+$/g, ""))
            fetch(packagePath+files[i], asBinary(destFile));
        }

        fetch(packageUrl, asBinary(destDir+"\\"+"package.json"))
    }

    function mkdirP(path){


    }

    function asText(){
        return function(response){
            return response.responseText;
        }
    }
    function asBinary(dest){
        return function (response){

            var stream = new ActiveXObject("ADODB.Stream");

            stream.Type = 1; // binary
            stream.Open();
            stream.Write(response.responseBody);

            stream.SaveToFile(dest, 2); // overwrite
        }
    }

    function fetch(url, destFn){
        var xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
        if (!xmlHttp)
            throw new Error("Could not create http request");
    
        xmlHttp.open('GET', url, false);
        
        var finishFn = function(){ throw new Error("Failed to fetch") };
        
        xmlHttp.onreadystatechange = function () {
            if (xmlHttp.readyState != 4)
                return;
            
            finishFn = function(){
                if (xmlHttp.status != 200)
                    throw new Error("Unexpected status "+xmlHttp.status);
                
                return destFn(xmlHttp);
            }
        };
        
        xmlHttp.send(null);
        
        return finishFn();
    }

    function fetchAsBinary(url, dest){

    }

    function fetchAsText(url){
        var xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
        if (!xmlHttp)
            throw new Error("Could not create http request");
    
        xmlHttp.open('GET', url, false);
        
        var finishFn = function(){ throw new Error("Failed to fetch") };
        
        xmlHttp.onreadystatechange = function () {
            if (xmlHttp.readyState != 4)
                return;
            
            finishFn = function(){
                if (xmlHttp.status != 200)
                    throw new Error("Unexpected status "+xmlHttp.status);
                
                return xmlHttp.responseText;
            }
        };
        
        xmlHttp.send(null);
        
        return finishFn();
    }
}


})();