// Generated by BUCKLESCRIPT VERSION 6.0.2, PLEASE EDIT WITH CARE
'use strict';

var List = require("bs-platform/lib/js/list.js");
var $$Array = require("bs-platform/lib/js/array.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Infix = require("../utils/infix.bs.js");
var React = require("react");
var Package = require("../model/package.bs.js");
var Metadata = require("../metadata.bs.js");
var ReactDOMRe = require("reason-react/lib/js/src/ReactDOMRe.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");

var font_style = {
  color: "white",
  fontFamily: "Montserrat, Helvetica Neue, Helvetica, sans-serif",
  fontSize: "1rem"
};

var style = ReactDOMRe.Style[/* combine */0](font_style, { });

function Design_system$Global(Props) {
  var children = Props.children;
  return React.createElement("section", {
              style: style
            }, children);
}

var Global = /* module */[
  /* font_style */font_style,
  /* style */style,
  /* make */Design_system$Global
];

var default_style = {
  margin: "0",
  padding: "1rem"
};

function Design_system$Container(Props) {
  var match = Props.style;
  var style = match !== undefined ? Caml_option.valFromOption(match) : { };
  var children = Props.children;
  return React.createElement("section", {
              style: ReactDOMRe.Style[/* combine */0](default_style, style)
            }, children);
}

var Container = /* module */[
  /* default_style */default_style,
  /* make */Design_system$Container
];

var style$1 = {
  backgroundColor: "#232323",
  color: "white",
  fontFamily: "Helvetica Neue, Helvetica, sans-serif",
  fontSize: "1em",
  fontWeight: "500",
  minWidth: "50px",
  padding: "5px",
  textTransform: "uppercase",
  borderRadius: "5px"
};

function Design_system$Label(Props) {
  var children = Props.children;
  return React.createElement("span", {
              style: style$1
            }, children);
}

var Label = /* module */[
  /* style */style$1,
  /* make */Design_system$Label
];

var style$2 = {
  color: "white",
  textDecoration: "none"
};

function Design_system$Link(Props) {
  var href = Props.href;
  var children = Props.children;
  return React.createElement(Design_system$Label, {
              children: React.createElement("a", {
                    style: style$2,
                    href: href
                  }, children)
            });
}

var Link = /* module */[
  /* style */style$2,
  /* make */Design_system$Link
];

var style$3 = {
  height: "50px",
  textAlign: "center"
};

function Design_system$Footer(Props) {
  var metadata = "Index built on " + Metadata.build_time;
  return React.createElement(Design_system$Container, {
              style: style$3,
              children: null
            }, "Built with ☕️ and ❤️ at", React.createElement(Design_system$Link, {
                  href: "https://src.technology",
                  children: "SRC"
                }), React.createElement("br", undefined), metadata);
}

var Footer = /* module */[
  /* style */style$3,
  /* make */Design_system$Footer
];

var style$4 = {
  background: "radial-gradient(ellipse at bottom, #1b2735 0%, #090a0f 100%)",
  bottom: "0",
  height: "100%",
  left: "0",
  overflow: "hidden",
  position: "fixed",
  right: "0",
  top: "0",
  width: "100%",
  zIndex: "-1"
};

function Design_system$Background(Props) {
  return React.createElement("div", {
              style: style$4
            });
}

var Background = /* module */[
  /* style */style$4,
  /* make */Design_system$Background
];

var style$5 = {
  margin: "2rem auto",
  textAlign: "center",
  width: "200px"
};

function Design_system$Loading(Props) {
  return React.createElement("div", {
              style: style$5
            }, "Loading...");
}

var Loading = /* module */[
  /* style */style$5,
  /* make */Design_system$Loading
];

function Design_system$Nav(Props) {
  var children = Props.children;
  return React.createElement("nav", undefined, children);
}

var Nav = /* module */[/* make */Design_system$Nav];

var style$6 = {
  display: "inline-block",
  maxWidth: "80px",
  padding: "1rem"
};

function Design_system$Logo(Props) {
  return React.createElement("img", {
              style: style$6,
              src: "./assets/logo.svg"
            });
}

var Logo = /* module */[
  /* style */style$6,
  /* make */Design_system$Logo
];

var style$7 = ReactDOMRe.Style[/* combine */0](font_style, {
      background: "none",
      border: "0",
      float: "right",
      margin: "0",
      outline: "0",
      padding: "1rem",
      width: "200px"
    });

function Design_system$Search_input(Props) {
  var onChange = Props.onChange;
  var placeholder = Props.placeholder;
  return React.createElement("input", {
              style: style$7,
              placeholder: placeholder,
              onChange: (function (e) {
                  return Curry._1(onChange, e.target.value);
                })
            });
}

var Search_input = /* module */[
  /* style */style$7,
  /* make */Design_system$Search_input
];

var style$8 = {
  margin: "0",
  padding: "0"
};

function Design_system$Tiny_title(Props) {
  var children = Props.children;
  return React.createElement("h5", {
              style: style$8
            }, children);
}

var Tiny_title = /* module */[
  /* style */style$8,
  /* make */Design_system$Tiny_title
];

var style$9 = {
  background: "#F9F9F9",
  color: "black",
  fontSize: "10px",
  listStyle: "none",
  padding: "0.5rem"
};

var name_style = {
  color: "black",
  display: "inline-block",
  fontWeight: "500",
  margin: "0 1em",
  overflow: "hidden",
  whiteSpace: "nowrap",
  width: "25%",
  textOverflow: "ellipsis"
};

var desc_style = {
  color: "black",
  display: "inline-block",
  margin: "0 1em",
  overflow: "hidden",
  whiteSpace: "nowrap",
  width: "60%",
  textOverflow: "ellipsis"
};

function Design_system$Package_card(Props) {
  var pkg = Props.pkg;
  return React.createElement("li", {
              style: style$9
            }, React.createElement("span", {
                  style: name_style
                }, pkg[/* name */0]), React.createElement("span", {
                  style: desc_style
                }, Infix.$$Option[/* <|> */0](pkg[/* description */1], "no description")), React.createElement(Design_system$Label, {
                  children: Package.Target[/* to_string */1](pkg[/* target */4])
                }));
}

var Package_card = /* module */[
  /* style */style$9,
  /* name_style */name_style,
  /* desc_style */desc_style,
  /* make */Design_system$Package_card
];

var style$10 = {
  padding: "0"
};

function Design_system$Package_list(Props) {
  var packages = Props.packages;
  return React.createElement("ul", {
              style: style$10
            }, $$Array.of_list(List.map((function (pkg) {
                        return React.createElement(Design_system$Package_card, {
                                    pkg: pkg
                                  });
                      }), packages)));
}

var Package_list = /* module */[
  /* style */style$10,
  /* make */Design_system$Package_list
];

var S = 0;

exports.S = S;
exports.Global = Global;
exports.Container = Container;
exports.Label = Label;
exports.Link = Link;
exports.Footer = Footer;
exports.Background = Background;
exports.Loading = Loading;
exports.Nav = Nav;
exports.Logo = Logo;
exports.Search_input = Search_input;
exports.Tiny_title = Tiny_title;
exports.Package_card = Package_card;
exports.Package_list = Package_list;
/* style Not a pure module */