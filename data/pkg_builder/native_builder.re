module Task = {};

let do_build = (_name, _root, _pkg, _path) => {
  ();
};

let build = (root, pkg) => {
  let name = Model.Package.name(pkg);

  let paths =
    Data_utils.Package.latest_major_versions(pkg)
    |> Seq.map(Model.Version.version)
    |> Seq.map(v => Printf.sprintf("./%s/%s/package", name, v))
    |> Seq.map(Fpath.v)
    |> List.of_seq;

  paths |> List.iter(do_build(name, root, pkg));

  ();
};
