module Target = {
  [@deriving yojson]
  type t = [ | `Universal | `Web | `Native | `Unknown];

  let of_string =
    fun
    | "universal" => `Universal
    | "web" => `Web
    | "native" => `Native
    | _ => `Unknown;

  let to_string =
    fun
    | `Universal => "universal"
    | `Web => "web"
    | `Native => "native"
    | `Unknown => "unknown";
};

[@deriving yojson]
type t = {
  name: string,
  description: option(string),
  versions: list(Version.t),
  keywords: list(string),
  target: Target.t,
};

let name = t => t.name;
let description = t => t.description;
let versions = t => t.versions;
let keywords = t => t.keywords;
let target = t => t.target;

let pp = (fmt, pkg) => {
  let name = pkg.name;
  let desc = Utils.Infix.Option.(pkg.description <|> "No description.");
  Format.fprintf(
    fmt,
    "@[(name %S) (description %S) (keywords %S) (versions ",
    name,
    desc,
    pkg.keywords |> String.concat(", "),
  );
  Format.pp_print_list(Version.pp, fmt, pkg.versions);
  Format.fprintf(fmt, ")@]");
};
