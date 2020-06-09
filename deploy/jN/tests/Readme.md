# JavaScript Test Environment

This folder contains simple test environment for jN.dll independent code.

## How to write tests

Test environment uses concept from `mocha` test framework for declaration
of tests.

Following code declares 4 unit tests for `func1` function under test. The 
test throws exception if the expectation is not fulfilled.

```javascript
describe("func1", function(){
	describe("works", function(){
		it("for 10", function(){
			func1(10);
		});
		it("for 20", function(){
			func1(20);
		});
	})
	
	describe("returns -1", function(){
		it("for input smaller than 0", function(){
			if (func1(-1) != -1)
				throw new Error("expect -1");
		});
		it("for input greater than 100", function(){
			if (func1(101) != -1)
				throw new Error("expect -1");
		});
	})
		
})

```

## How to execute tests

Following command executes all tests declared in *.spec.js files of 
current directory.

```sh
$> cscript /nologo runner.cli.js

Test Runner

[succeded]  process env exists
[succeded]  process cwd returns path
[succeded]  process pid exits
```