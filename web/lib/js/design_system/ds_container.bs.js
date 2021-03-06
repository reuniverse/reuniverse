// Generated by BUCKLESCRIPT VERSION 6.0.2, PLEASE EDIT WITH CARE
'use strict';

var React = require("react");
var ReactDOMRe = require("reason-react/lib/js/src/ReactDOMRe.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");

var default_style = {
  margin: "0",
  padding: "1rem"
};

function Ds_container(Props) {
  var match = Props.style;
  var style = match !== undefined ? Caml_option.valFromOption(match) : { };
  var children = Props.children;
  return React.createElement("section", {
              style: ReactDOMRe.Style[/* combine */0](default_style, style)
            }, children);
}

var S = 0;

var make = Ds_container;

exports.S = S;
exports.default_style = default_style;
exports.make = make;
/* react Not a pure module */
