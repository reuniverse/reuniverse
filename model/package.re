[@deriving yojson]
type t = {
  name: string,
  description: option(string),
  versions: list(Version.t),
};

let pp = (fmt, pkg) => {
  let name = pkg.name;
  let desc = Utils.Infix.Option.(pkg.description <|> "No description.");
  Format.fprintf(
    fmt,
    "@[(name \"%S\") (description \"%S\") (versions ",
    name,
    desc,
  );
  Format.pp_print_list(Version.pp, fmt, pkg.versions);
  Format.fprintf(fmt, ")@]");
};
