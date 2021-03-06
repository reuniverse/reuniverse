// Generated by BUCKLESCRIPT VERSION 6.0.2, PLEASE EDIT WITH CARE
'use strict';

var List = require("bs-platform/lib/js/list.js");
var $$Array = require("bs-platform/lib/js/array.js");
var Js_json = require("bs-platform/lib/js/js_json.js");
var Package = require("../model/package.bs.js");
var Repromise = require("@aantron/repromise/lib/js/src/js/repromise.js");
var Caml_array = require("bs-platform/lib/js/caml_array.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");

function parse(json) {
  var obj = Belt_Option.getExn(Js_json.decodeObject(json));
  var package_count = Belt_Option.getExn(Js_json.decodeNumber(obj["package_count"])) | 0;
  var packages = List.map((function (json_pkg) {
          var x = Js_json.decodeString(Caml_array.caml_array_get(Belt_Option.getExn(Js_json.decodeArray(json_pkg["target"])), 0));
          return /* record */[
                  /* name */Belt_Option.getExn(Js_json.decodeString(json_pkg["name"])),
                  /* description */Js_json.decodeString(json_pkg["description"]),
                  /* versions : [] */0,
                  /* keywords : [] */0,
                  /* target */Package.Target[/* of_string */0](Belt_Option.getWithDefault(x, "unknown").toLowerCase())
                ];
        }), List.map(Belt_Option.getExn, List.map(Js_json.decodeObject, $$Array.to_list(Belt_Option.getExn(Js_json.decodeArray(obj["packages"]))))));
  return /* record */[
          /* package_count */package_count,
          /* packages */packages
        ];
}

function load(param) {
  return Repromise.Rejectable[/* map */5](parse, Repromise.Rejectable[/* fromJsPromise */10](fetch("https://raw.githubusercontent.com/reuniverse/reuniverse/master/packages/index.json").then((function (prim) {
                        return prim.json();
                      }))));
}

var unwrap = Belt_Option.getExn;

exports.unwrap = unwrap;
exports.parse = parse;
exports.load = load;
/* Package Not a pure module */
