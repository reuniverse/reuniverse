let pkg_root = Fpath.v("./packages");
let unpackaged_dir_name = Fpath.v("package");

let skip_unknown_package = pkg => {
  switch (Model.Package.(pkg.target)) {
  | `Unknown => false
  | _ => true
  };
};

let build_one = (root, pkg) => {
  switch (Model.Package.(pkg.target)) {
  | `Web => Web_builder.build(root, pkg)
  | `Native
  | `Universal => Native_builder.build(root, pkg)
  | `Unknown =>
    Logs.info(m =>
      m(
        "Ignoring package %s since we don't know how to compile for target Unknown :shrugs:",
        Model.Package.(pkg.name),
      )
    )
  };
};

let build_all_packages = index => {
  open Rresult;
  let pwd = Bos.OS.Dir.current() >>| (pwd => Fpath.(pwd /\/ pkg_root));
  let pkgs = Model.Index.(index.packages);
  Logs.info(m => m("Building %d packages...", pkgs |> List.length));

  pwd
  >>= (
    root => {
      Web_builder.prepare_environment(root)
      >>| (
        _ => {
          pkgs
          |> List.filter(skip_unknown_package)
          |> List.iter(build_one(root));
        }
      );
    }
  );
};
