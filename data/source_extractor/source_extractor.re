let pkg_root = Fpath.v("./packages");
let unzipped_tarball_name = Fpath.v("source.tar");
let tarball_file_name = Fpath.(unzipped_tarball_name |> add_ext(".gz"));
let unpackaged_dir_name = Fpath.v("package");

let skip_unknown_package = pkg => {
  switch (Model.Package.(pkg.target)) {
  | `Unknown => false
  | _ => true
  };
};

let tar_path = (pkg, v) => {
  let name = Model.Package.(pkg.name) |> Fpath.v;
  let ver = Model.Version.(v.version) |> Fpath.v;
  Fpath.(pkg_root /\/ name /\/ ver /\/ tarball_file_name);
};

let extracted_sources_path = (pkg, v) => {
  let name = Model.Package.(pkg.name) |> Fpath.v;
  let ver = Model.Version.(v.version) |> Fpath.v;
  Fpath.(pkg_root /\/ name /\/ ver /\/ unpackaged_dir_name);
};

let has_tarball = (pkg, v) => tar_path(pkg, v) |> Bos.OS.File.exists;

let has_extracted_sources = (pkg, v) =>
  extracted_sources_path(pkg, v) |> Bos.OS.Dir.exists;

let skip_already_extracted = pkg => {
  Data_utils.Package.latest_major_versions(pkg)
  |> List.of_seq
  |> List.filter(v =>
       switch (has_tarball(pkg, v), has_extracted_sources(pkg, v)) {
       | (Ok(true), Ok(false)) =>
         /* Tarball exists, but it hasn't been extracted, so we should keep this */
         true
       | _ =>
         Logs.info(m =>
           m(
             "Skipped %s since it is already extracted",
             extracted_sources_path(pkg, v) |> Fpath.to_string,
           )
         );
         /* Otherwise, the tarball doesn't exist, or we already have extracted this */
         false;
       }
     )
  |> List.map(v => (pkg, v));
};

let uncompress = data => {
  open Rresult;
  let input_buffer = Bytes.create(0xFFFF);
  let output_buffer = Bytes.create(0xFFFF);
  let window =
    Decompress.Window.create(
      ~crc=Decompress.Window.crc32,
      ~witness=Decompress.Buffer.bytes,
    );
  let pos = ref(0);
  let res = Buffer.create(String.length(data));
  Decompress.Gzip_inflate.bytes(
    input_buffer,
    output_buffer,
    input_buffer => {
      let n = min(0xFFFF, String.length(data) - pos^);
      Bytes.blit_string(data, pos^, input_buffer, 0, n);
      pos := pos^ + n;
      n;
    },
    (output_buffer, len) => {
      Buffer.add_subbytes(res, output_buffer, 0, len);
      0xFFFF;
    },
    Decompress.Gzip_inflate.default(~witness=Decompress.Buffer.bytes, window),
  )
  >>| (_ => Buffer.contents(res))
  |> Rresult.R.reword_error((e: Decompress.error_g_inflate) => {
       let msg = {
         let buf = Buffer.create(1024);
         let fmt = Format.formatter_of_buffer(buf);
         Decompress.Gzip_inflate.pp_error(fmt, e);
         Format.pp_print_flush(fmt, ());
         buf |> Buffer.contents;
       };
       `Decompression_error(msg);
     });
};

let extract_one:
  ((Model.Package.t, Model.Version.t)) =>
  result(
    (Model.Package.t, Model.Version.t),
    [ | `Extraction_error(string, Model.Package.t, Model.Version.t)],
  ) =
  ((pkg, v)) => {
    open Rresult;
    let tar = tar_path(pkg, v);
    let version_folder = tar |> Fpath.parent;
    let sha = v |> Model.Version.repository |> Model.Repository.ref;
    let repo_name = v |> Model.Version.repository |> Model.Repository.name;
    let extract_path =
      Fpath.(version_folder /\/ v(repo_name ++ "-" ++ sha) |> to_string);
    let final_path =
      Fpath.(version_folder /\/ unpackaged_dir_name |> to_string);
    Bos.Cmd.(
      v("tar")
      % "-zxf"
      % (tar |> Fpath.to_string)
      % "-C"
      % (version_folder |> Fpath.to_string)
    )
    |> Bos.OS.Cmd.run
    >>= (_ => Bos.OS.Cmd.run(Bos.Cmd.(v("mv") % extract_path % final_path)))
    >>| (_ => (pkg, v))
    |> Rresult.R.reword_error((`Msg(e)) => `Extraction_error((e, pkg, v)));
  };

let handle_extraction_result:
  result(
    (Model.Package.t, Model.Version.t),
    [ | `Extraction_error(string, Model.Package.t, Model.Version.t)],
  ) =>
  unit =
  res => {
    switch (res) {
    | Ok((pkg, v)) =>
      Logs.info(m =>
        m(
          "Successfully extracted %s/%s",
          Model.Package.(pkg.name),
          Model.Version.(v.version),
        )
      )
    | Error(`Extraction_error(msg, pkg, v)) =>
      Logs.err(m =>
        m(
          "Failed to extract %s/%s: %s",
          Model.Package.(pkg.name),
          Model.Version.(v.version),
          msg,
        )
      )
    };
  };

let extract_all_sources = index => {
  let pkgs = Model.Index.(index.packages);
  Logs.info(m =>
    m("Attempting to extract %d package sources...", pkgs |> List.length)
  );
  pkgs
  |> List.filter(skip_unknown_package)
  |> List.map(skip_already_extracted)
  |> List.flatten
  |> List.map(extract_one)
  |> List.iter(handle_extraction_result);
  Ok();
};
