let version_of_npm_version: Npm.Package_version.t => Model.Version.t =
  version => {
    version: version.version,
    download_url: version.dist.tarball |> Uri.to_string,
    integrity_shasum: version.dist.shasum |> Npm.Sha.to_string,
  };

let package_of_npm_pkg:
  (Npm.Package.t, Model.Package.Target.t) => Model.Package.t =
  (pkg, target) => {
    let name = pkg.name;
    let description = pkg.description;
    let versions = pkg.versions |> List.map(version_of_npm_version);
    let keywords = pkg.keywords;
    {name, description, versions, keywords, target};
  };

let annotate_with_targets: (Npm.Package.t, string) => _ =
  (pkg, version) => {
    open Lwt_result.Infix;

    let is_native =
      Unpkg.Api.V1.file(~name=pkg.name, ~version, ~path="dune-project")
      >|= (
        maybeFile =>
          switch (maybeFile) {
          | None => []
          | Some(_) => [`Native]
          }
      );

    let is_web =
      Unpkg.Api.V1.file(~name=pkg.name, ~version, ~path="bsconfig.json")
      >|= (
        maybeFile =>
          switch (maybeFile) {
          | None => []
          | Some(_) => [`Web]
          }
      );

    is_web
    >>= (
      web => {
        is_native
        >|= (
          native => {
            switch (web, native) {
            | ([`Web], []) => `Web
            | ([], [`Native]) => `Native
            | ([`Web], [`Native]) => `Universal
            | _ => `Unknown
            };
          }
        );
      }
    );
  };

let scan: list(Npm.Search.Query.param) => Model.Index.t =
  keywords => {
    Logs.app(m =>
      m(
        "Scanning npm for packages including keywords: %s",
        keywords
        |> List.map(Npm.Search.Query.to_string)
        |> String.concat(", "),
      )
    );

    let search_by_param = param => {
      let query = Npm.Search.Query.{params: [param]};
      Lwt.catch(
        () => Npm.Api.V1.search(~query, ~from=0, ~size=10000),
        exn => {
          Logs.err(m => m("%s", Printexc.to_string(exn)));
          Logs.app(m => m("Continuing..."));
          Npm.Search.empty |> Lwt_result.return;
        },
      );
    };

    let pkg_names = {
      open Lwt.Infix;
      let table = Hashtbl.create(1024);
      keywords
      |> List.to_seq
      |> Seq.map(search_by_param)
      |> Lwt_stream.of_seq
      |> Lwt_stream.iter_p(search_res =>
           search_res
           |> Lwt_result.map(res => {
                let pkgs = Npm.Search.(res.results);
                Logs.app(m => m("Found %d packages", res.total));
                pkgs
                |> List.iter(result => {
                     let name = Npm.Search.(result.package.name);
                     switch (Hashtbl.find_opt(table, name)) {
                     | None => Hashtbl.add(table, name, name)
                     | _ => ()
                     };
                   });
              })
           >>= (_ => Lwt.return())
         )
      |> Lwt_main.run;
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
        "Downloaded metadata for %d packages, proceeding to refine metadata...",
        Hashtbl.stats(pkgs).num_bindings,
      )
    );

    let refined_pkgs = {
      open Lwt.Infix;
      let table = Hashtbl.create(1024);
      pkgs
      |> Hashtbl.to_seq_values
      |> Seq.map(pkg => {
           let latest_version =
             Npm.Package.(pkg.versions)
             |> List.map(x => Npm.Package_version.(x.version))
             |> List.map(Semver.of_string)
             |> List.fold_left(
                  (acc, semver) =>
                    switch (acc, semver) {
                    | (None, s) => s
                    | (s, None) => s
                    | (Some(oldver), Some(newver)) =>
                      switch (Semver.greater_than(oldver, newver)) {
                      | true => Some(oldver)
                      | _ => Some(newver)
                      }
                    },
                  None,
                )
             |> (
               v => {
                 switch (v) {
                 | Some(semver) => semver |> Semver.to_string
                 | None => ""
                 };
               }
             );

           annotate_with_targets(pkg, latest_version)
           |> Lwt_result.map(target => {
                Logs.info(m =>
                  m(
                    "Refined %s@%s as %s",
                    pkg.name,
                    latest_version,
                    target |> Model.Package.Target.to_string,
                  )
                );

                (pkg, target);
              });
         })
      |> Lwt_stream.of_seq
      |> Lwt_stream.iter_p(res =>
           res
           |> Lwt_result.map(((pkg, target)) => {
                let pkg = package_of_npm_pkg(pkg, target);
                Hashtbl.add(table, pkg.name, pkg);
              })
           >>= (_ => Lwt.return())
         )
      |> Lwt_main.run;
      table;
    };

    Logs.app(m =>
      m(
        "Refined metadata for %d packages, proceeding to build index...",
        Hashtbl.stats(pkgs).num_bindings,
      )
    );

    Model.Index.make(
      ~packages=refined_pkgs |> Hashtbl.to_seq_values |> List.of_seq,
    );
  };
