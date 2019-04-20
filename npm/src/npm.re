/*
 module Version = {
   type t = string;

   let of_string = x => x;

   let to_string = x => x;
 };
 */

/*
 module Package_evaluation = {
   type quality = {
     carefulness: float,
     tests: float,
     health: float,
     branding: float,
   };

   type popularity = {
     community_interest: float,
     downloads_count: float,
     downloads_acceleration: float,
     dependents_count: float,
   };

   type maintenance = {
     releases_frequency: float,
     commits_frequency: float,
     open_issues: float,
     issues_distribution: float,
   };

   type t = {
     quality,
     popularity,
     maintenance,
   };
 };
 */

module Repository = {
  type t = {
    kind: string,
    url: string,
    directory: string,
  };

  let from_json: Yojson.Basic.t => t =
    json => {
      Yojson.Basic.{
        kind: Util.(json |> member("kind") |> to_string),
        url: Util.(json |> member("url") |> to_string),
        directory: Util.(json |> member("directory") |> to_string),
      };
    };
};

/*
 module Author = {
   type t = {
     username: string,
     email: string,
   };
 };
 */

module Package = {
  /*
   module Flag = {
     type t =
       | Deprecated
       | Unstable
       | Insecure;
   };

   module Score = {
     type detailed_score = {
       quality: float,
       popularity: float,
       maintenance: float,
     };

     type t = {
       final: float,
       detail: detailed_score,
     };
   };
   */

  type t = {
    name: string,
    description: string,
    repository: Repository.t,
    /*
     keywords: list(string),
     maintainers: list(Author.t),
     publisher: Author.t,
     scope: string,
     version: Version.t,
     */
  };

  let from_json: Yojson.Basic.t => t =
    json => {
      Yojson.Basic.{
        name: Util.(json |> member("name") |> to_string),
        description: Util.(json |> member("description") |> to_string),
        repository:
          Util.(json |> member("repository") |> Repository.from_json),
      };
    };
};

module Search = {
  module Query = {
    // | `Exclude(Package.Flag.t)
    // | `Include(Package.Flag.t)
    // | `Score_effect(float)
    // | `Quality_weight(float)
    // | `Popularity_weight(float)
    // | `Maintenance_weight(float)
    // | `Scope(string)
    // | `Author(string)
    // | `Maintainer(string)
    // | `Boost_exact_matches(bool)
    type param = [ | `Keywords(list(string))];

    type t = {
      params: list(param),
      /*
       from: option(int),
       size: option(int),
       */
    };

    let param_to_pair =
      fun
      | `Keywords(kws) => ("keywords", kws);

    let to_query_params = t => {
      t.params |> List.map(param_to_pair);
    };
  };

  type search_result = {
    package: Package.t,
    /*
     flags: Package.Flag.t,
     score: Package.Score.t,
     search_score: float,
     */
  };

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
          |> member("results")
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
  type t = {
    // analyzed_at: string,
    collected,
    // score: Package.score,
  };
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
  module V2 = {
    let base_url = "https://api.npms.io/v2";

    let tls_config = Httpkit_lwt.Client.Https.Config.no_auth();

    let search:
      (~query: Search.Query.t, ~from: int, ~size: int) =>
      Lwt_result.t(Search.t, _) =
      (~query, ~from as _, ~size as _) => {
        open Lwt_result.Infix;

        let search_url = base_url ++ "/search" |> Uri.of_string;
        let query = query |> Search.Query.to_query_params;
        let req =
          Httpkit.Client.Request.create(
            ~headers=[],
            `POST,
            Uri.with_query(search_url, query),
          );

        Httpkit_lwt.Client.(
          req
          |> Https.send(~config=tls_config)
          >>= Response.body
          |> Lwt_result.map(body =>
               body |> Yojson.Basic.from_string |> Search.from_json
             )
        );
      };

    let packages: list(string) => Lwt_result.t(list(Package_response.t), _) =
      names => {
        open Lwt_result.Infix;

        let search_url = base_url ++ "/mget" |> Uri.of_string;
        let body =
          "["
          ++ (
            names
            |> List.fold_left(
                 (acc, name) => "\"" ++ name ++ "\", " ++ acc,
                 "",
               )
          )
          ++ "]"; // ["a","b","c"]

        let req =
          Httpkit.Client.Request.create(
            ~headers=[
              ("Accept", "application/json"),
              ("Content-Type", "application/json"),
            ],
            ~body,
            `POST,
            search_url,
          );

        Httpkit_lwt.Client.(
          req
          |> Https.send(~config=tls_config)
          >>= Response.body
          |> Lwt_result.map(body =>
               body
               |> Yojson.Basic.from_string
               |> Yojson.Basic.Util.to_assoc
               |> List.map(((_name, pkg)) =>
                    pkg |> Package_response.from_json
                  )
             )
        );
      };
  };
};
