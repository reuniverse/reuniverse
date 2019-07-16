[@deriving yojson]
type t = {
  version: string,
  repository: Repository.t,
  download_url: string,
  integrity_shasum: string,
};

let version = t => t.version;
let download_url = t => t.download_url;
let integrity_shasum = t => t.integrity_shasum;
let repository = t => t.repository;

let pp = (fmt, v) =>
  Format.fprintf(
    fmt,
    "@[(version %S) (download_url %S) (integrity_shasum %S)@]",
    v.version,
    v.download_url,
    v.integrity_shasum,
  );
