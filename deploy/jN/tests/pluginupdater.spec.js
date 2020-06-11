require("describe");
require("expect");

var PluginUpdater = require("../lib/PluginUpdater").PluginUpdater;

describe("PluginUpdater", function(){
    describe("Ctor", function(){
        it("works", function (){
            new PluginUpdater();
        })
    })

    describe("getPluginsInTheRegistry", function(){
        it("returns registred plugins", function(){
           var pu = new PluginUpdater("https://raw.githubusercontent.com/jn-npp-plugin/plugin-registry/master/plugins.json", "");
           var plugins = pu.getPluginsInTheRegistry();

           expect(plugins).not().eq(null);

           var xmlPlugin = plugins["XML"];
           expect(xmlPlugin).not().eq(null);
           expect(typeof(xmlPlugin.url)).eq("string");
        })
    })

    describe("updatePlugin", function(){
        it("throws on not existing plugin", function(){
            var pu = new PluginUpdater("https://raw.githubusercontent.com/jn-npp-plugin/plugin-registry/master/plugins.json", "");
 
            expect(function(){ pu.updatePlugin("this plugin does not exist")}).throws();
        });

        it("downloads plugin declaration and corresponding files", function(){
           var pu = new PluginUpdater("https://raw.githubusercontent.com/jn-npp-plugin/plugin-registry/master/plugins.json", "");
           var plugins = pu.getPluginsInTheRegistry();

           expect(plugins).not().eq(null);

           var xmlPlugin = plugins["XML"];
           expect(xmlPlugin).not().eq(null);
           expect(typeof(xmlPlugin.url)).eq("string");
        })
    })
})
