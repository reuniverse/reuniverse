[@deriving yojson]
type t = {
  name: string,
  description: option(string),
  versions: list(Version.t),
  keywords: list(string),
};

let pp = (fmt, pkg) => {
  let name = pkg.name;
  let desc = Utils.Infix.Option.(pkg.description <|> "No description.");
  Format.fprintf(
    fmt,
    "@[(name %S) (description %S) (keywords %S) (versions ",
    name,
    desc,
    pkg.keywords |> String.concat(", ")
  );
  Format.pp_print_list(Version.pp, fmt, pkg.versions);
  Format.fprintf(fmt, ")@]");
};
