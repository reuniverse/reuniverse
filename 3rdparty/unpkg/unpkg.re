module Response = {
  let is_error: string => _ =
    str => {
      switch (str |> String.split_on_char(' ')) {
      | ["Cannot", "find", ..._] => Error(`Not_found)
      | _ => Ok()
      };
    };
};

module File = {
  type t = {
    name: string,
    version: string,
    path: string,
    contents: string,
  };
};

module Api = {
  module Httpkit = Httpkit_lwt_unix_httpaf;

  module V1 = {
    let base_url = "https://unpkg.com";
    let base_port = 443;

    let headers = [
      ("accept", "application/json"),
      ("content-type", "application/json"),
      ("user-agent", "reuniverse/httpkit"),
    ];

    let file:
      (~name: string, ~version: string, ~path: string) =>
      Lwt_result.t(option(File.t), _) =
      (~name, ~version, ~path) => {
        open Lwt_result.Infix;

        let uri =
          Printf.sprintf("%s/%s@%s/%s", base_url, name, version, path)
          |> Uri.of_string;

        let req = Httpkit.Request.create(~headers, `GET, uri);

        Logs.debug(m => m("%s", uri |> Uri.to_string));

        req
        |> Httpkit.Client.Https.send
        >>= Httpkit.Client.Response.body
        >>= (
          body => {
            switch (Response.is_error(body)) {
            | Error(`Not_found) => Lwt_result.return(None)
            | Ok(_) =>
              Some(File.{name, version, path, contents: body})
              |> Lwt_result.return
            };
          }
        );
      };
  };
};
