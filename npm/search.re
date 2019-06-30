module Package = {
  type t = {name: string};

  let from_json: Yojson.Basic.t => t =
    json => {
      Yojson.Basic.{name: Util.(json |> member("name") |> to_string)};
    };
};

module Query = {
  type param = [ | `Keywords(list(string))];

  type t = {params: list(param)};

  let param_to_pair =
    fun
    | `Keywords(kws) => ("text", ["keywords:" ++ String.concat(",", kws)]);

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
             {package: res |> member("package") |> Package.from_json}
           )
      );
    {total, results};
  };
