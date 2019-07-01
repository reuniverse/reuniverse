let version_of_npm_version: Npm.Package_version.t => Model.Version.t =
  version => {
    version: version.version,
    download_url: version.dist.tarball|> Uri.to_string,
    integrity_shasum: version.dist.shasum |> Npm.Sha.to_string,
  };

let package_of_npm_pkg: Npm.Package.t => Model.Package.t =
  pkg => {
    let name = pkg.name;
    let description = pkg.description;
    let versions = pkg.versions |> List.map(version_of_npm_version);
    let keywords = pkg.keywords;
    {name, description, versions, keywords};
  };

let scan: list(string) => Model.Index.t =
  keywords => {
    Logs.app(m =>
      m(
        "Scanning npm for packages including keywords: %s",
        keywords |> String.concat(", "),
      )
    );

    let search_by_keyword = keyword => {
      let query = Npm.Search.Query.{params: [`Keywords([keyword])]};
      let get_pkgs = Npm.Api.V1.search(~query, ~from=0, ~size=10000);
      switch (get_pkgs |> Lwt_main.run) {
      | exception e =>
        Logs.err(m => m("%s", Printexc.to_string(e)));
        Logs.app(m => m("Continuing..."));
        [];
      | Error(_err) => []
      | Ok(pkgs) => pkgs.results
      };
    };

    let pkg_names = {
      let table = Hashtbl.create(1024);
      keywords
      |> List.map(search_by_keyword)
      |> List.concat
      |> List.iter(pkg => {
           let name = pkg.Npm.Search.package.name;
           switch (Hashtbl.find_opt(table, name)) {
           | None => Hashtbl.add(table, name, name)
           | _ => ()
           };
         });
      table;
    };

    Logs.app(m =>
      m(
        "Found %d packages, proceeding to query for their details...",
        Hashtbl.stats(pkg_names).num_bindings,
      )
    );

    let pkgs = {
      open Lwt.Infix;
      let table = Hashtbl.create(1024);
      pkg_names
      |> Hashtbl.to_seq_keys
      |> Seq.map(Npm.Api.V1.package)
      |> Lwt_stream.of_seq
      |> Lwt_stream.iter_p(pkg_res =>
           pkg_res
           |> Lwt_result.map(pkg =>
                switch (pkg) {
                | Some(pkg) =>
                  let name = Npm.Package.(pkg.name);
                  Logs.info(m => m("Saving package %s", name));
                  Hashtbl.add(table, pkg.name, pkg);
                | None => ()
                }
              )
           >>= (_ => Lwt.return())
         )
      |> Lwt_main.run;
      table;
    };

    Logs.app(m =>
      m(
        "Downloaded metadata for %d packages, proceeding to build index...",
        Hashtbl.stats(pkgs).num_bindings,
      )
    );

    Model.Index.make(
      ~packages=
        pkgs
        |> Hashtbl.to_seq_values
        |> Seq.map(package_of_npm_pkg)
        |> List.of_seq,
    );
  };
