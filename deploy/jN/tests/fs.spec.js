require("describe");
var expect = require("expect").expect;
require("../lib/fs");
require("../lib/Nodejs")

describe("fs", function (){
	describe("mkdtempSync", function(){
		it("returns concat with prefix", function(){
            if (!/^prefix-.+/.test(fs.mkdtempSync("prefix-")))
                throw new Error("Wrong return value");            
		});
    });	
    describe("existsSync", function(){
        it("returns true for existing directory", function(){
            expect(fs.existsSync(process.env["TEMP"])).eq(true)
        })
        it("returns false for not existing directory", function(){
            expect(fs.existsSync(process.env["TEMP"]+"/123321-fs-spec-dir")).eq(false)
        })
    })
    describe("mkdirSync", function(){
		it("creates directory if not exists", function(){
            var tmpDir = fs.mkdtempSync(process.env["TEMP"]+"/");
            fs.mkdirSync(tmpDir);
            
            expect(fs.existsSync(tmpDir)).eq(true);

            fs.rmdirSync(tmpDir);           
        });
        it("creates directory does not fail if exists", function(){
            var tmpDir = process.env["TEMP"];
            fs.mkdirSync(tmpDir);          
        });
        it("creates directory recursive", function(){
            var tmpDir = process.env["TEMP"] + "/fs.spec.rec\\ursive";
            fs.mkdirSync(tmpDir, {recursive:true});       
            
            expect(fs.existsSync(process.env["TEMP"] + "/fs.spec.rec")).eq(true);
            expect(fs.existsSync(process.env["TEMP"] + "/fs.spec.rec\\ursive")).eq(true);
            
            fs.rmdirSync(tmpDir);
		});
    });
    describe("rmdirSync", function(){
		it("removes existing directory", function(){
            var tmpDir = fs.mkdtempSync(process.env["TEMP"]+"/");
            fs.mkdirSync(tmpDir);
            
            fs.rmdirSync(tmpDir);  
            
            expect(fs.existsSync(tmpDir)).eq(false);
        });
	});
})