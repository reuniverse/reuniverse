open Rresult;

let docs_root = Fpath.v("_docs");
let pkg_root = Fpath.v("packages");
let unpackaged_dir_name = Fpath.v("package");

let skip_unknown_package = pkg => {
  switch (Model.Package.(pkg.target)) {
  | `Unknown => false
  | _ => true
  };
};

let gen_one = (root, pkg, pkg_v) => {
  let name = Model.Package.name(pkg);
  let version = Model.Version.version(pkg_v);
  let pkg_subpath = Fpath.(v(name) /\/ v(version));
  let output_dir = Fpath.(root /\/ docs_root /\/ pkg_subpath);
  let src_path =
    Fpath.(root /\/ pkg_root /\/ pkg_subpath /\/ unpackaged_dir_name);
  let top_level_name = {
    let parts = name |> String.split_on_char('/');
    let len = parts |> List.length;
    List.nth(parts, len - 1) |> Str2.to_camel_case;
  };
  (
    switch (Model.Package.(pkg.target)) {
    | `Web =>
      Bos.OS.Dir.create(output_dir)
      >>| (_ => Logs.debug(m => m("[%s] Created output path", name)))
      >>= (() => Bos.OS.Dir.set_current(src_path))
      >>| (() => Logs.debug(m => m("[%s] Moved to directory", name)))
      >>= (() => Bsdoc.Cmd.build(~output_dir, top_level_name))
      >>| (() => Logs.info(m => m("[%s] Generated docs", name)))
    | t => Error(`Unsupported_target(t |> Model.Package.Target.to_string))
    }
  )
  |> (
    fun
    | Ok () => Logs.info(m => m("[%s] Successfully generated docs.", name))
    | Error(`Unsupported_target(t)) =>
      Logs.err(m => m("[%s] Error: Unsupported target %s", name, t))
    | Error(`Bsdoc_gen_error(e))
    | Error(`Msg(e)) => Logs.err(m => m("[%s] Error: %s", name, e))
  );
  ();
};

let generate_docs = index => {
  let pkgs = Model.Index.(index.packages);
  Logs.info(m =>
    m("Generating docs for %d packages...", pkgs |> List.length)
  );

  Bos.OS.Dir.current()
  >>| (
    root =>
      pkgs
      |> List.to_seq
      |> Seq.filter(skip_unknown_package)
      |> Seq.flat_map(pkg =>
           Data_utils.Package.latest_major_versions(pkg) |> Seq.map(v => (pkg, v))
         )
      |> List.of_seq
      |> List.iter(((pkg, v)) => gen_one(root, pkg, v))
  );
};
