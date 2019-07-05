module Npm_scanner = Npm_scanner;

let default_index_path = "./packages/index.json" |> Fpath.v;

let common_keywords = [
  `Keywords(["reason"]),
  `Keywords(["bucklescript"]),
  `Keywords(["reasonml"]),
  `Keywords(["ocaml"]),
  `Text("reason-"),
];

let build = () => {
  let npm_pkgs = Npm_scanner.scan(common_keywords);
  // let github_pkgs = scan_github();
  npm_pkgs;
};

let save = (~path, index) => {
  Logs.info(m => m("Serializing index..."));
  let index_json =
    index |> Model.Index.to_yojson |> Yojson.Safe.pretty_to_string;
  Logs.app(m =>
    m("Saving index.json file at %s", path |> Fpath.to_string)
  );
  Bos.OS.File.write(path, index_json);
};

let load = (~path) => {
  open Rresult;
  Logs.info(m => m("Loading index from %s...", path |> Fpath.to_string));
  Bos.OS.File.read(path)
  >>| Yojson.Safe.from_string
  >>| Model.Index.of_yojson
  >>= Rresult.R.reword_error(e => `Parse_error(e));
};
