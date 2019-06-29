module Httpkit = Httpkit_lwt_unix_httpaf;

module Links = {
  type t = {
    homepage: option(string),
    repository: option(string),
  };

  let from_json: Yojson.Basic.t => t =
    json => {
      Yojson.Basic.{
        homepage: Util.(json |> member("homepage") |> to_string_option),
        repository: Util.(json |> member("repository") |> to_string_option),
      };
    };
};

module Package = {
  type t = {
    name: string,
    description: option(string),
    links: Links.t,
  };

  let from_json: Yojson.Basic.t => t =
    json => {
      Yojson.Basic.{
        name: Util.(json |> member("name") |> to_string),
        description: Util.(json |> member("description") |> to_string_option),
        links: Util.(json |> member("links") |> Links.from_json),
      };
    };
};

module Search = {
  module Query = {
    type param = [ | `Keywords(list(string))];

    type t = {params: list(param)};

    let param_to_pair =
      fun
      | `Keywords(kws) => (
          "text",
          ["keywords:" ++ String.concat(",", kws)],
        );

    let to_query_params = t => {
      t.params |> List.map(param_to_pair);
    };
  };

  type search_result = {package: Package.t};

  type t = {
    total: int,
    results: list(search_result),
  };

  let from_json: Yojson.Basic.t => t =
    json => {
      open Yojson.Basic;
      let total = Util.(json |> member("total") |> to_int);
      let results =
        Util.(
          json
          |> member("objects")
          |> to_list
          |> List.map(res =>
               {
                 package: Util.(res |> member("package") |> Package.from_json),
               }
             )
        );
      {total, results};
    };
};

module Package_response = {
  type collected = {metadata: Package.t};
  type t = {collected};
  let from_json: Yojson.Basic.t => t =
    json => {
      open Yojson.Basic;
      let metadata =
        Util.(
          json
          |> member("collected")
          |> member("metadata")
          |> Package.from_json
        );
      {
        collected: {
          metadata: metadata,
        },
      };
    };
};

module API = {
  module V1 = {
    let base_host = "registry.npmjs.org";
    let base_url = "https://registry.npmjs.org/-/v1";
    let base_port = 443;

    let search:
      (~query: Search.Query.t, ~from: int, ~size: int) =>
      Lwt_result.t(Search.t, _) =
      (~query, ~from, ~size) => {
        open Lwt_result.Infix;

        let search_url = base_url ++ "/search" |> Uri.of_string;
        let query =
          [
            ("size", [string_of_int(size)]),
            ("from", [string_of_int(from)]),
          ]
          @ (query |> Search.Query.to_query_params);
        let uri = Uri.with_query(search_url, query);
        let req = Httpkit.Request.create(`GET, uri);

        Logs.app(m => m("%s", uri |> Uri.to_string));

        req
        |> Httpkit.Client.Https.send
        >>= Httpkit.Client.Response.body
        >|= (body => body |> Yojson.Basic.from_string |> Search.from_json);
      };

    let package: string => Lwt_result.t(list(Package_response.t), _) =
      name => {
        open Lwt_result.Infix;

        let search_url = base_url ++ "/" ++ name |> Uri.of_string;

        let req =
          Httpkit.Request.create(
            ~headers=[
              ("accept", "application/json"),
              ("content-type", "application/json"),
            ],
            `POST,
            search_url,
          );

        req
        |> Httpkit.Client.Https.send
        >>= Httpkit.Client.Response.body
        >|= (
          body =>
            body
            |> Yojson.Basic.from_string
            |> Yojson.Basic.Util.to_assoc
            |> List.map(((_name, pkg)) => pkg |> Package_response.from_json)
        );
      };
  };
};
