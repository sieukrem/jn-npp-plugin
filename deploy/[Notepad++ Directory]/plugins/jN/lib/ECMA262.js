/**
	Some functions missed in Windows JavaScript Engine
*/

/**
	calls fn for each el in an Array. Returns an new array of results of fn calls
	XXX: had to make this function closer to ECMA-262 5th spec since original
	implementation breaks compatibility for Underscore library
*/
if (!Array.prototype.forEach) {
	Array.prototype.forEach = function(fn, scope) {
		var result = [];
		for (var i = 0, len = this.length; i < len; ++i) {
			result.push(fn.call(scope || this, this[i], i, this));
		}

		return result;
	};
}
/**
 * returns index of el in an Array, otherwise -1
 * XXX ECMA-262 implemetation
 */
if (!Array.prototype.indexOf) {
    Array.prototype.indexOf = function (searchElement /*, fromIndex */ ) {
        "use strict";
        if (this == null) {
            throw new TypeError();
        }
        var t = Object(this);
        var len = t.length >>> 0;
        if (len === 0) {
            return -1;
        }
        var n = 0;
        if (arguments.length > 0) {
            n = Number(arguments[1]);
            if (n != n) { // shortcut for verifying if it's NaN
                n = 0;
            } else if (n != 0 && n != Infinity && n != -Infinity) {
                n = (n > 0 || -1) * Math.floor(Math.abs(n));
            }
        }
        if (n >= len) {
            return -1;
        }
        var k = n >= 0 ? n : Math.max(len - Math.abs(n), 0);
        for (; k < len; k++) {
            if (k in t && t[k] === searchElement) {
                return k;
            }
        }
        return -1;
    };
}

/**
 * removes whitespace from both sides of a string.
 * JavaScript Version 1.8 implemetation
 */
if (!String.prototype.trim){
	String.prototype.trim = function()
	{
        "use strict";
        if (this == null) 
            throw new TypeError();
		
		return this.replace(/^\s+|\s+$/gm,'');
	}
}
/**
 * Add ECMA262-5 method binding if not supported natively
 */
if (!('bind' in Function.prototype)) {
    Function.prototype.bind= function(owner) {
        var that= this;
        if (arguments.length<=1) {
            return function() {
                return that.apply(owner, arguments);
            };
        } else {
            var args= Array.prototype.slice.call(arguments, 1);
            return function() {
                return that.apply(owner, arguments.length===0? args : args.concat(Array.prototype.slice.call(arguments)));
            };
        }
    };
}
/**
 * Add ECMA262-5 method isArray if not supported natively
 */
if(!Array.isArray) {
  Array.isArray = function(arg) {
    return Object.prototype.toString.call(arg) === '[object Array]';
  };
}

/**
 * Add ECMA262-5.1 method filter if not supported natively
 */
if (!Array.prototype.filter)
{
  Array.prototype.filter = function(fun /*, thisArg */)
  {
    "use strict";

    if (this === void 0 || this === null)
      throw new TypeError();

    var t = Object(this);
    var len = t.length >>> 0;
    if (typeof fun != "function")
      throw new TypeError();

    var res = [];
    var thisArg = arguments.length >= 2 ? arguments[1] : void 0;
    for (var i = 0; i < len; i++)
    {
      if (i in t)
      {
        var val = t[i];

        // NOTE: Technically this should Object.defineProperty at
        //       the next index, as push can be affected by
        //       properties on Object.prototype and Array.prototype.
        //       But that method's new, and collisions should be
        //       rare, so use the more-compatible alternative.
        if (fun.call(thisArg, val, i, t))
          res.push(val);
      }
    }

    return res;
  };
}
/**
 * Add ECMA262-5 method map if not supported natively
 */
if (!Array.prototype.map)
{
  Array.prototype.map = function(fun /*, thisArg */)
  {
    "use strict";

    if (this === void 0 || this === null)
      throw new TypeError();

    var t = Object(this);
    var len = t.length >>> 0;
    if (typeof fun !== "function")
      throw new TypeError();

    var res = new Array(len);
    var thisArg = arguments.length >= 2 ? arguments[1] : void 0;
    for (var i = 0; i < len; i++)
    {
      // NOTE: Absolute correctness would demand Object.defineProperty
      //       be used.  But this method is fairly new, and failure is
      //       possible only if Object.prototype or Array.prototype
      //       has a property |i| (very unlikely), so use a less-correct
      //       but more portable alternative.
      if (i in t)
        res[i] = fun.call(thisArg, t[i], i, t);
    }

    return res;
  };
}
/**
 * Add ECMA262-5 method some if not supported natively
 */
if (!Array.prototype.some)
{
  Array.prototype.some = function(fun /*, thisArg */)
  {
    'use strict';

    if (this === void 0 || this === null)
      throw new TypeError();

    var t = Object(this);
    var len = t.length >>> 0;
    if (typeof fun !== 'function')
      throw new TypeError();

    var thisArg = arguments.length >= 2 ? arguments[1] : void 0;
    for (var i = 0; i < len; i++)
    {
      if (i in t && fun.call(thisArg, t[i], i, t))
        return true;
    }

    return false;
  };
}
/**
 * Add ECMA262-5 method some if not supported natively
 */
if (!Array.prototype.lastIndexOf) {
  Array.prototype.lastIndexOf = function(searchElement /*, fromIndex*/) {
    'use strict';

    if (this == null) {
      throw new TypeError();
    }

    var n, k,
        t = Object(this),
        len = t.length >>> 0;
    if (len === 0) {
      return -1;
    }

    n = len;
    if (arguments.length > 1) {
      n = Number(arguments[1]);
      if (n != n) {
        n = 0;
      }
      else if (n != 0 && n != (1 / 0) && n != -(1 / 0)) {
        n = (n > 0 || -1) * Math.floor(Math.abs(n));
      }
    }

    for (k = n >= 0
          ? Math.min(n, len - 1)
          : len - Math.abs(n); k >= 0; k--) {
      if (k in t && t[k] === searchElement) {
        return k;
      }
    }
    return -1;
  };
}

// From https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/keys
if (!Object.keys) {
  Object.keys = (function () {
    'use strict';
    var hasOwnProperty = Object.prototype.hasOwnProperty,
        hasDontEnumBug = !({toString: null}).propertyIsEnumerable('toString'),
        dontEnums = [
          'toString',
          'toLocaleString',
          'valueOf',
          'hasOwnProperty',
          'isPrototypeOf',
          'propertyIsEnumerable',
          'constructor'
        ],
        dontEnumsLength = dontEnums.length;

    return function (obj) {
      if (typeof obj !== 'object' && (typeof obj !== 'function' || obj === null)) {
        throw new TypeError('Object.keys called on non-object');
      }

      var result = [], prop, i;

      for (prop in obj) {
        if (hasOwnProperty.call(obj, prop)) {
          result.push(prop);
        }
      }

      if (hasDontEnumBug) {
        for (i = 0; i < dontEnumsLength; i++) {
          if (hasOwnProperty.call(obj, dontEnums[i])) {
            result.push(dontEnums[i]);
          }
        }
      }
      return result;
    };
  }());
}

/**
	Define global JSON object if native one is not 
	available. Native JSON object has more stable 
	implementation.
*/
(function(){
	if (typeof(JSON) != "undefined")
		return;
		
	var rexQuote = /\"/g;
	var rexSlash = /\\/g;
	
	function direct(v){
		return v;
	};
	function unk2str(unk){
		var type = typeof(unk);
		return conv[(type == "object" && unk.join) ?"array":type](unk);
	};
	var	conv = {
		"boolean"	: direct,
		"string"	: function(str){	return  '"'+str.replace(rexSlash, '\\\\').replace(rexQuote, '\\"')+'"';},
		"number" 	: direct,
		"object" 	:function(obj){
						var r = "", comma = "";
						for(var el in obj){
							r+= comma+unk2str(el)+':'+unk2str(obj[el]);
							comma = ",";
						}
						return  "{"+r+"}";
					},
		"array"		:function(arr){ 
						var r="",comma="";
						for(var i=0, c=arr.length; i<c; i++){
							r+= comma+unk2str(arr[i]);
							comma = ",";
						}
						return "["+r+"]";
					},
		"undefined"	:direct,
		"function"	:direct
	};
	
	JSON = {
		stringify : function(value){
			return unk2str(value);
		},
		parse : function(str){
			return eval("(" + str + ")");
		}
	};
})();
