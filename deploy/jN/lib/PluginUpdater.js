
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
        var plugin = plugins["name"];
        if (!plugin)
            throw new Error("Plugin not found");

        
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