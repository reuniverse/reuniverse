module Search = Search;

module Sha: {
  type t;
  let of_string: string => t;
  let to_string: t => string;
} = {
  type t = string;
  let of_string = x => x;
  let to_string = x => x;
};

module Author = {
  exception Can_not_parse_author_from_json(Yojson.Basic.t);
  type t = {
    name: option(string),
    email: option(string),
  };
  let from_json: Yojson.Basic.t => t =
    json => {
      Yojson.Basic.(
        switch (json) {
        | `String(str) => {name: Some(str), email: None}
        | `Assoc(_) =>
          let name = Util.(json |> member("name") |> to_string_option);
          let email = Util.(json |> member("email") |> to_string_option);
          {name, email};
        | _ => raise(Can_not_parse_author_from_json(json))
        }
      );
    };
};

module Package_distribution = {
  type t = {
    shasum: Sha.t,
    tarball: Uri.t,
  };
  let from_json: Yojson.Basic.t => t =
    json => {
      open Yojson.Basic;
      let shasum =
        Util.(json |> member("shasum") |> to_string |> Sha.of_string);
      let tarball =
        Util.(json |> member("tarball") |> to_string |> Uri.of_string);
      {shasum, tarball};
    };
};

module Package_version = {
  type t = {
    _id: string,
    name: string,
    version: string,
    git_head: option(Sha.t),
    dist: Package_distribution.t,
    author: option(Author.t),
  };

  let from_json: Yojson.Basic.t => t =
    json => {
      open Yojson.Basic;

      let _id = Util.(json |> member("_id") |> to_string);

      let name = Util.(json |> member("name") |> to_string);

      let version = Util.(json |> member("version") |> to_string);
      let git_head =
        Util.(
          json
          |> member("gitHead")
          |> to_string_option
          |> (
            fun
            | None => None
            | Some(s) => Some(Sha.of_string(s))
          )
        );
      let dist =
        Util.(json |> member("dist") |> Package_distribution.from_json);
      let author =
        Util.(json |> member("author") |> to_option(Author.from_json));

      {_id, name, version, git_head, dist, author};
    };
};

module Package = {
  type t = {
    _id: string,
    name: string,
    description: option(string),
    keywords: list(string),
    versions: list(Package_version.t),
  };

  let from_json: Yojson.Basic.t => t =
    json => {
      open Yojson.Basic;

      let _id = Util.(json |> member("_id") |> to_string);

      let name = Util.(json |> member("name") |> to_string);

      let description =
        Util.(json |> member("description") |> to_string_option);

      let keywords =
        Util.(json |> member("keywords") |> to_list |> List.map(to_string));

      let versions =
        Util.(
          json
          |> member("versions")
          |> to_assoc
          |> List.map(((_, v)) => v)
          |> List.map(Package_version.from_json)
        );

      {_id, name, description, versions, keywords};
    };
};

module Response = {
  let is_error: Yojson.Basic.t => _ =
    json => {
      open Yojson.Basic;

      let error = Util.(json |> member("error") |> to_string_option);

      switch (error) {
      | None => `Not_error
      | Some("Not found") => `Error(`Not_found)
      | Some(msg) => `Error(`Message(msg))
      };
    };
};

module Api = {
  module Httpkit = Httpkit_lwt_unix_httpaf;

  module V1 = {
    let base_host = "registry.npmjs.org";
    let base_url = "https://registry.npmjs.org";
    let base_port = 443;

    let headers = [
    ("User-Agent", "reason/npm")];

    let search:
      (~query: Search.Query.t, ~from: int, ~size: int) =>
      Lwt_result.t(Search.t, _) =
      (~query, ~from, ~size) => {
        open Lwt_result.Infix;

        let search_url = base_url ++ "/-/v1/search" |> Uri.of_string;
        let query =
          [
            ("size", [string_of_int(size)]),
            ("from", [string_of_int(from)]),
          ]
          @ (query |> Search.Query.to_query_params);
        let uri = Uri.with_query(search_url, query);
        let req = Httpkit.Request.create(~headers, `GET, uri);

        Logs.app(m => m("%s", uri |> Uri.to_string));

        req
        |> Httpkit.Client.Https.send
        >>= Httpkit.Client.Response.body
        >|= (body => body |> Yojson.Basic.from_string |> Search.from_json);
      };

    let package: string => Lwt_result.t(option(Package.t), _) =
      name => {
        open Lwt_result.Infix;

        let search_url = base_url ++ "/" ++ name |> Uri.of_string;

        let req =
          Httpkit.Request.create(
            ~headers=[
              ("accept", "application/json"),
              ("content-type", "application/json"),
            ],
            `GET,
            search_url,
          );

        Logs.debug(m => m("Requesting: %s", search_url |> Uri.to_string));

        req
        |> Httpkit.Client.Https.send
        >>= Httpkit.Client.Response.body
        >>= (
          body => {
            let json = body |> Yojson.Basic.from_string;

            switch (Response.is_error(json)) {
            | `Error(`Not_found) => Lwt_result.return(None)
            | `Error(`Message(msg)) => Lwt_result.fail(`Api_error(msg))
            | `Not_error =>
              switch (body |> Yojson.Basic.from_string |> Package.from_json) {
              | exception e =>
                Logs.err(m => {
                  let exn = Printexc.to_string(e);
                  m("Failed to parse package %s: %s", name, exn);
                });
                Logs.err(m => m("%s", search_url |> Uri.to_string));
                Logs.err(m => m("%s", json |> Yojson.Basic.to_string));
                Lwt_result.fail(`Parse_error(e));
              | pkg => Lwt_result.return(Some(pkg))
              }
            };
          }
        );
      };
  };
};
