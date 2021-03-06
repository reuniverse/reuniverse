// Generated by BUCKLESCRIPT VERSION 6.0.2, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Format = require("bs-platform/lib/js/format.js");

function version(t) {
  return t[/* version */0];
}

function download_url(t) {
  return t[/* download_url */2];
}

function integrity_shasum(t) {
  return t[/* integrity_shasum */3];
}

function repository(t) {
  return t[/* repository */1];
}

function pp(fmt, v) {
  return Curry._3(Format.fprintf(fmt, /* Format */[
                  /* Formatting_gen */Block.__(18, [
                      /* Open_box */Block.__(1, [/* Format */[
                            /* End_of_format */0,
                            ""
                          ]]),
                      /* String_literal */Block.__(11, [
                          "(version ",
                          /* Caml_string */Block.__(3, [
                              /* No_padding */0,
                              /* String_literal */Block.__(11, [
                                  ") (download_url ",
                                  /* Caml_string */Block.__(3, [
                                      /* No_padding */0,
                                      /* String_literal */Block.__(11, [
                                          ") (integrity_shasum ",
                                          /* Caml_string */Block.__(3, [
                                              /* No_padding */0,
                                              /* Char_literal */Block.__(12, [
                                                  /* ")" */41,
                                                  /* Formatting_lit */Block.__(17, [
                                                      /* Close_box */0,
                                                      /* End_of_format */0
                                                    ])
                                                ])
                                            ])
                                        ])
                                    ])
                                ])
                            ])
                        ])
                    ]),
                  "@[(version %S) (download_url %S) (integrity_shasum %S)@]"
                ]), v[/* version */0], v[/* download_url */2], v[/* integrity_shasum */3]);
}

exports.version = version;
exports.download_url = download_url;
exports.integrity_shasum = integrity_shasum;
exports.repository = repository;
exports.pp = pp;
/* Format Not a pure module */
