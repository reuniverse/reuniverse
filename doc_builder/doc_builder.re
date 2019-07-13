module Httpkit = Httpkit_lwt_unix_httpaf;

let pkg_root = Fpath.v("./packages");
let source_file_name = Fpath.v("source.tar.gz");

let skip_unknown_package = pkg => {
  switch (Model.Package.(pkg.target)) {
  | `Unknown => false
  | _ => true
  };
};

let skip_already_downloaded_versions = (pkg, version) => {
  open Model.Package;
  open Model.Version;
  let source_dir = Fpath.(pkg_root /\/ v(pkg.name) /\/ v(version.version));
  let source_path = Fpath.(source_dir /\/ source_file_name);
  let source_exists = Bos.OS.File.exists(source_path);
  switch (source_exists) {
  | Ok(true) =>
    Logs.info(m => m("Skipping package %s/%s", pkg.name, version.version));
    false; /* false will skip! */
  | Ok(false) => true /* true will keep!  */
  | Error(`Msg(msg)) =>
    Logs.err(m =>
      m("Error checking package %s/%s: %s", pkg.name, version.version, msg)
    );
    false;
  };
};

let pick_newest_major_versions = (name, versions) => {
  let majors = Hashtbl.create(8);
  versions
  |> List.iter(v =>
       switch (Model.Version.(v.version |> Semver.of_string)) {
       | None => ()
       | Some(semver) =>
         let major = Semver.(semver.major);
         Logs.debug(m => m("Checking major version %d", major));
         switch (Hashtbl.find_opt(majors, major)) {
         | None =>
           Logs.debug(m => m("Not found! Adding %d...", major));
           Hashtbl.replace(majors, major, v);
         | Some(v') =>
           Logs.debug(m => m("Found (unparsed): %s", v'.version));
           switch (Model.Version.(v'.version |> Semver.of_string)) {
           | None =>
             Logs.debug(m =>
               m("Saving %s instead", semver |> Semver.to_string)
             );
             Hashtbl.replace(majors, major, v);
           | Some(semver') =>
             Logs.debug(m => m("Found: %s", semver' |> Semver.to_string));
             switch (Semver.greater_than(semver, semver')) {
             | true => Hashtbl.replace(majors, major, v)
             | _ => ()
             };
           };
         };
       }
     );
  let count = majors |> Hashtbl.length;
  Logs.info(m => m("Found %d major versions for %s", count, name));
  majors |> Hashtbl.to_seq_values;
};

let download_one: (Model.Package.t, Model.Version.t) => _ =
  (pkg, pkg_v) => {
    open Lwt_result.Infix;
    let source_dir = Fpath.(pkg_root /\/ v(pkg.name) /\/ v(pkg_v.version));
    let source_path = Fpath.(source_dir /\/ source_file_name);
    Logs.debug(m => m("GET %s", pkg_v.download_url));
    pkg_v.download_url
    |> Uri.of_string
    |> Httpkit.Request.create(`GET)
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
    );
  };

let download_all_versions = pkg => {
  Model.Package.(pkg.versions)
  |> pick_newest_major_versions(pkg.name)
  |> Seq.filter(skip_already_downloaded_versions(pkg))
  |> Seq.map(download_one(pkg))
  |> Lwt_stream.of_seq
  |> Lwt_stream.iter_p(res =>
       Lwt.Infix.(
         res
         >>= (
           s => {
             switch (s) {
             | Ok(version) =>
               Logs.app(m =>
                 m(
                   "Downloaded %s/%s into ./packages/%s",
                   Model.Package.(pkg.name),
                   Model.Version.(version.version),
                   Model.Package.(pkg.name),
                 )
               )
             | Error(`Download_error(version, `Msg(msg))) =>
               Logs.err(m =>
                 m(
                   "Failed to download %s/%s: %s",
                   Model.Package.(pkg.name),
                   Model.Version.(version.version),
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
    |> Lwt_stream.iter_s(res => res >>= (() => Lwt.return()))
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

let build_from_index: Model.Index.t => _ =
  index => {
    Ok(index |> download_all_packages |> Lwt_main.run);
  };
