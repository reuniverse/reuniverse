open Rresult;

module Task = {
  let normalize_compiler = (root, cache_folder, pkg_root) =>
    Bos.OS.Dir.set_current(Fpath.(root /\/ pkg_root))
    >>= (
      _ =>
        Yarn.Cmd.add(
          ~cache_folder,
          [("bs-platform", "6"), ("bsdoc", "6.0.2-alpha")],
        )
    );

  let install_deps = (root, cache_folder, pkg_root) =>
    Bos.OS.Dir.set_current(Fpath.(root /\/ pkg_root))
    >>= Yarn.Cmd.install(~cache_folder);

  let clean = (root, pkg_root) =>
    Bos.OS.Dir.set_current(Fpath.(root /\/ pkg_root)) >>= Bsb.Cmd.clean_world;

  let compile = (root, pkg_root) =>
    Bos.OS.Dir.set_current(Fpath.(root /\/ pkg_root)) >>= Bsb.Cmd.make_world;
};

let do_build = (name, root, _pkg, cache_folder, path) => {
  Task.normalize_compiler(root, cache_folder, path)
  >>| (() => Logs.info(m => m("[%s] Normalized compiler", name)))
  >>= (() => Task.install_deps(root, cache_folder, path))
  >>| (() => Logs.info(m => m("[%s] Installed dependencies", name)))
  >>= (() => Task.clean(root, path))
  >>| (() => Logs.info(m => m("[%s] Cleaned compilation artifacts", name)))
  >>= (() => Task.compile(root, path))
  >>| (() => Logs.info(m => m("[%s] Compiled package", name)))
  |> (
    fun
    | Ok () => Logs.info(m => m("[%s] Successfully built.", name))
    | Error(`Yarn_install_error(e))
    | Error(`Yarn_add_error(e))
    | Error(`Bsb_clean_world_error(e))
    | Error(`Bsb_make_world_error(e))
    | Error(`Npm_link_error(e))
    | Error(`Msg(e)) => Logs.err(m => m("[%s] Error: %s", name, e))
  );
};

let prepare_environment = root => {
  let cache_folder = Fpath.(root /\/ v(".yarn_cache"));
  let global = true;
  Yarn.Cmd.add(
    ~global,
    ~cache_folder,
    [("bs-platform", "6"), ("bsdoc", "6.0.2-alpha")],
  );
};

let build = (root, pkg) => {
  let name = Model.Package.name(pkg);
  let cache_folder = Fpath.(root /\/ v(".yarn_cache"));

  let paths =
    Data_utils.Package.latest_major_versions(pkg)
    |> Seq.map(Model.Version.version)
    |> Seq.map(v => Printf.sprintf("./%s/%s/package", name, v))
    |> Seq.map(Fpath.v)
    |> List.of_seq;

  paths |> List.iter(do_build(name, root, pkg, cache_folder));

  ();
};
