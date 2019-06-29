module Package = {
  type t = {
    name: string,
    repo_url: string,
  };
  let make = (~name, ~repo_url) => {name, repo_url};

  let pp = (fmt, pkg) =>
    Format.fprintf(fmt, "@[(%S %S)@]", pkg.name, pkg.repo_url);
};

type t = {
  package_count: int,
  packages: list(Package.t),
};

let empty = {package_count: 0, packages: []};

let make = (~packages) => {
  package_count: packages |> List.length,
  packages:
    packages |> List.sort((a, b) => Package.(String.compare(a.name, b.name))),
};

let pp = (fmt, index) => {
  Format.fprintf(
    fmt,
    "@[(package_count \"%d\")\n(packages @[",
    index.package_count,
  );
  Format.pp_print_list(Package.pp, fmt, index.packages);
  Format.fprintf(fmt, ")@]@]");
};
