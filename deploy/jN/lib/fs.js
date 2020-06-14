(function(){
    
    var fso = new ActiveXObject("Scripting.FileSystemObject");

    fs = {
        existsSync: function(path){
            return fso.FileExists(path) || fso.FolderExists(path);
        },
        mkdirSync: function(path, options){
            if (!!options)
                throw new Error("Not implemented for options")

            if (fso.FolderExists(path))
                return;
            
            fso.CreateFolder(path);
        },
        rmdirSync: function(path, options){
            if (!!options)
                throw new Error("Not implemented for options")
            
            var ignoreReadOnlyAttr = true;
            fso.DeleteFolder(path, ignoreReadOnlyAttr);
        },
        mkdtempSync: function(prefix, options){
            if(!!options)
                throw new Error("Not implemented for options")
            
            return prefix + fso.GetTempName();
        }
    }

    module.exports.fs = fs;
})();