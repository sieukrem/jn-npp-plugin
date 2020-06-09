describe = function (name, testCtxFn){
	var previous = describe.state;

	describe.state = {
		name: name,
		tests:[]
	};
	
	previous.tests.push(describe.state);
	
	if ( typeof(testCtxFn) === "function")
		testCtxFn();	
	
	describe.state = previous;
}

it = function(name, testFn){
	describe.state.tests.push({
		name: name,
		testFn: testFn
	});
}

describe.state = {
	tests:[]
};

function listTests(state){
	var names = [];
	
	var curName = "";
	if (!!state.name)
		curName = state.name + ".";
	
	for(var i=0; i< state.tests.length; i++){
		names.push(curName+state.tests[i].name);
		
		if (!!state.tests)
			names = names.concat(listTests(state.tests[i]).map(function(n){ return curName+n; }))
	}
	return names;
}

runTests = function(fixture){
	if (!fixture){
		return runTests(describe.state);
	}
	
	var result = { name:fixture.name }
		
	if (fixture.testFn){
		try{
			fixture.testFn();
			result.result = "succeded";
		}catch(e){
			result.result = "failed";
		}		
		return result;
	}
	
	result.tests = [];
	for(var i=0; i< fixture.tests.length; i++){
		if (!!fixture.tests){
			result.tests.push(runTests(fixture.tests[i]))
		}
	}
	return result;
}