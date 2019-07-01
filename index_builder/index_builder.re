module Npm_scanner = Npm_scanner;

let common_keywords = ["reason", "bucklescript", "reasonml"];

let build = () => {
  let npm_pkgs = Npm_scanner.scan(common_keywords);
  // let github_pkgs = scan_github();
  npm_pkgs;
};

let save = index => {
  Logs.info(m => m("Serializing index..."));
  let index_json = index |> Model.Index.to_yojson |> Yojson.Safe.pretty_to_string;
  let filename = Fpath.v("./packages/index.json");
  Logs.app(m => m("Saving index.json file at %s", filename |> Fpath.to_string));
  Bos.OS.File.write(filename, index_json);
};
