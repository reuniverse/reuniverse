module Httpkit = Httpkit_lwt_unix_httpaf;

let max_concurrency = 10;

let url_of_repository = repo => {
  let uri = Model.Repository.url(repo) |> Uri.of_string;
  switch (Model.Repository.kind(repo)) {
  | `GitLab =>
    let path = Uri.path(uri) |> String.split_on_char('.') |> List.hd;
    let path =
      path
      ++ "/-/archive/"
      ++ Model.Repository.ref(repo)
      ++ "/"
      ++ Model.Repository.name(repo)
      ++ "-"
      ++ Model.Repository.ref(repo)
      ++ ".tar.gz";

    "https://" ++ Uri.host_with_default(uri) ++ path |> Uri.of_string;

  | `GitHub =>
    let path = Uri.path(uri) |> String.split_on_char('.') |> List.hd;
    let path = path ++ "/tar.gz/" ++ Model.Repository.ref(repo);

    "https://codeload." ++ Uri.host_with_default(uri) ++ path |> Uri.of_string;
  };
};

let pkg_root = Fpath.v("./packages");
let source_file_name = Fpath.v("source.tar.gz");

let skip_unknown_package = pkg => {
  switch (Model.Package.(pkg.target)) {
  | `Unknown => false
  | _ => true
  };
};

let skip_already_downloaded_versions = (pkg, ver) => {
  open Model.Package;
  open Model.Version;
  let source_dir = Fpath.(pkg_root /\/ v(pkg.name) /\/ v(ver.version));
  let source_path = Fpath.(source_dir /\/ source_file_name);
  let source_exists = Bos.OS.File.exists(source_path);
  switch (source_exists) {
  | Ok(true) =>
    Logs.info(m => m("Skipping package %s/%s", pkg.name, ver.version));
    false; /* false will skip! */
  | Ok(false) => true /* true will keep!  */
  | Error(`Msg(msg)) =>
    Logs.err(m =>
      m("Error checking package %s/%s: %s", pkg.name, ver.version, msg)
    );
    false;
  };
};

let download_one: (Model.Package.t, Model.Version.t) => _ =
  (pkg, pkg_v) => {
    open Lwt_result.Infix;
    Logs.info(m => m("Downloading %s/%s...", pkg.name, pkg_v.version));
    let source_dir = Fpath.(pkg_root /\/ v(pkg.name) /\/ v(pkg_v.version));
    let source_path = Fpath.(source_dir /\/ source_file_name);
    let url = url_of_repository(pkg_v.repository);

    Logs.debug(m => m("GET %s", url |> Uri.to_string));

    Lwt.catch(
      () =>
        Httpkit.Request.create(`GET, url)
        |> Httpkit.Client.Https.send
        >>= Httpkit.Client.Response.body
        >>= (
          body => {
            Rresult.(
              Bos.OS.Dir.create(~path=true, source_dir)
              >>= (_ => Bos.OS.File.write(source_path, body))
              |> Rresult.R.map(() => pkg_v)
              |> Rresult.R.reword_error(e => `Download_error((pkg_v, e)))
              |> Lwt_result.lift
            );
          }
        ),
      e => Lwt_result.fail(`Download_exception((pkg_v, e))),
    );
  };

let download_all_versions = pkg => {
  Data_utils.Package.latest_major_versions(pkg)
  |> Seq.filter(skip_already_downloaded_versions(pkg))
  |> Seq.map(download_one(pkg))
  |> Lwt_stream.of_seq
  |> Lwt_stream.iter_n(~max_concurrency, res =>
       Lwt.Infix.(
         res
         >>= (
           s => {
             switch (s) {
             | Ok(version) =>
               Logs.app(m =>
                 m(
                   "Downloaded %s/%s into ./packages/%s",
                   Model.Package.name(pkg),
                   Model.Version.version(version),
                   Model.Package.name(pkg),
                 )
               )
             | Error(`Download_exception(version, ex)) =>
               Logs.err(m =>
                 m(
                   "Failed to download %s/%s: %s",
                   Model.Package.name(pkg),
                   Model.Version.version(version),
                   ex |> Printexc.to_string,
                 )
               )
             | Error(`Download_error(version, `Msg(msg))) =>
               Logs.err(m =>
                 m(
                   "Failed to download %s/%s: %s",
                   Model.Package.name(pkg),
                   Model.Version.version(version),
                   msg,
                 )
               )
             | Error(_) => Logs.err(m => m("Other error occurred"))
             };
             Lwt.return();
           }
         )
       )
     );
};

let download_all_packages = index => {
  Lwt.Infix.(
    Model.Index.(index.packages)
    |> List.to_seq
    |> Seq.filter(skip_unknown_package)
    |> Seq.map(download_all_versions)
    |> Lwt_stream.of_seq
    |> Lwt_stream.iter_n(~max_concurrency, res =>
         res >>= (() => Lwt.return())
       )
    >|= (
      () => {
        Logs.app(m =>
          m(
            "Downloaded %d packages into ./packages",
            Model.Index.(index.packages) |> List.length,
          )
        );
      }
    )
  );
};

let download_from_index: Model.Index.t => _ =
  index => {
    Ok(index |> download_all_packages |> Lwt_main.run);
  };
