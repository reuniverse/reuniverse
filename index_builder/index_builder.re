module Index = Index;

let common_keywords = ["reason", "bucklescript", "reasonml"];

let scan_npm: unit => Index.t =
  () => {
    let search_by_keyword = keyword => {
      let query = Npm.Search.Query.{params: [`Keywords([keyword])]};
      let get_pkgs = Npm.API.V1.search(~query, ~from=0, ~size=10000);
      switch (get_pkgs |> Lwt_main.run) {
      | exception e =>
        Logs.err(m => m("%s", Printexc.to_string(e)));
        Logs.app(m => m("Continuing..."));
        [];
      | Error(_err) => []
      | Ok(pkgs) => pkgs.results
      };
    };

    let pkgs = Hashtbl.create(1024);

    common_keywords
    |> List.map(search_by_keyword)
    |> List.concat
    |> List.map(pkg => pkg.Npm.Search.package)
    |> List.iter(pkg =>
         switch (
           Npm.Package.(pkg.links.repository),
           Hashtbl.find_opt(pkgs, pkg.name),
         ) {
         | (Some(repo_url), None) =>
           Index.Package.make(~name=pkg.name, ~repo_url)
           |> Hashtbl.add(pkgs, pkg.name)
         | _ => ()
         }
       );

    Index.make(~packages=pkgs |> Hashtbl.to_seq_values |> List.of_seq);
  };

let build = () => {
  let npm_pkgs = scan_npm();

  npm_pkgs;
};
