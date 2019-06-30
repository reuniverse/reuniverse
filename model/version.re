module Uri = {
  include Uri;

  let to_yojson: t => Yojson.Safe.t =
    x => "\"" ++ Uri.to_string(x) ++ "\"" |> Yojson.Safe.from_string;

  let of_yojson: Yojson.Safe.t => Ppx_deriving_yojson_runtime.error_or(t) =
    json =>
      Ok(
        Utils.Infix.Option.(
          Yojson.Safe.Util.(json |> to_string_option <|> "") |> Uri.of_string
        ),
      );
};

[@deriving yojson]
type t = {
  version: string,
  download_url: Uri.t,
  integrity_shasum: string,
};

let pp = (fmt, v) =>
  Format.fprintf(
    fmt,
    "@[(version %S) (download_url %S) (integrity_shasum %S)@]",
    v.version,
    v.download_url |> Uri.to_string,
    v.integrity_shasum,
  );
