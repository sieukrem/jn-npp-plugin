expect = function(testee){
    var expectation = function(_testee, _actionname, _neg) {
        this.throws = function(){
            var error = null;
            try{
                _testee();
            }catch(e){
                error = e;
            }
            
            if (error == null){
                if (!!_neg)
                    return;
                
                throw new Error("Expected to throw, but was not");
            } else {
                if (!!_neg)
                    throw new Error("Expected not to throw, but was");
            }
        };

        this.eq = function(val){
            if (!!_neg && _testee != val || !_neg && _testee == val)
                return;

            throw new Error("Expected "+ _testee+" " + (!!_neg?"not ":"")+ _actionname +", but found "+val);
        }

        this.not = function(){ 
            return new expectation(_testee, _actionname, !_neg); 
        }
    }
    return new expectation(testee, testee, false);
}

module.exports.expect = expect;