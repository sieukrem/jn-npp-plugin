require("../lib/Nodejs");

(function(){
    testenv = function(testeeFn){
        return function (){
            var tmpDir = fs.mkdirSync(fs.mkdtempSync(process.env["TEMP"]+"\\"));
            
            var env = {
                directory: tmpDir
            };
            
            try{
                testeeFn(env);
            }finally{
                fs.rmdirSync(tmpDir);
            }
        }
    }

    module.exports.testenv = testenv;
})();

