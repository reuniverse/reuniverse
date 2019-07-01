[@deriving yojson]
type t = {
  version: string,
  download_url: string,
  integrity_shasum: string,
};

let pp = (fmt, v) =>
  Format.fprintf(
    fmt,
    "@[(version %S) (download_url %S) (integrity_shasum %S)@]",
    v.version,
    v.download_url,
    v.integrity_shasum,
  );
