module Httpkit = Httpkit_lwt_unix_httpaf;

let download_one: (Model.Package.t, Model.Version.t) => _ =
  (pkg, pkg_v) => {
    open Lwt_result.Infix;
    let source_dir = Fpath.(v("./packages") /\/ v(pkg.name) /\/ v(pkg_v.version));
    let source_path = Fpath.(source_dir /\/ v("source.tar.gz"));
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
  |> List.to_seq
  |> Seq.map(download_one(pkg))
  |> Lwt_stream.of_seq
  |> Lwt_stream.iter_s(res =>
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
  Model.Index.(index.packages)
  |> List.to_seq
  |> Seq.map(download_all_versions)
  |> Lwt_stream.of_seq
  |> Lwt_stream.iter_p(res =>
       Lwt.Infix.(
         res
         >>= (
           () => {
             Logs.app(m =>
               m(
                 "Downloaded %d packages into ./packages",
                 Model.Index.(index.packages) |> List.length,
               )
             );
             Lwt.return();
           }
         )
       )
     );
};

let build_from_index: Model.Index.t => _ =
  index => {
    Ok(index |> download_all_packages |> Lwt_main.run);
  };
