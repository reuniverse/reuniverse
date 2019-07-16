[@deriving yojson]
type kind = [ | `GitHub | `GitLab];

[@deriving yojson]
type t = {
  kind,
  name: string,
  url: string,
  ref: string,
};

let make = (~kind=`GitHub, ~ref, url) => {
  kind,
  url,
  ref,
  name: {
    let parts = url |> String.split_on_char('/');
    let len = parts |> List.length;
    let name_with_ext =
      List.nth(parts, len - 1) |> String.split_on_char('.');
    List.nth(name_with_ext, 0);
  },
};

let kind = t => t.kind;
let url = t => t.url;
let ref = t => t.ref;
let name = t => t.name;
