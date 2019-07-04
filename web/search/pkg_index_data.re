let unwrap = Belt.Option.getExn;

let parse: Js.Json.t => Model.Index.t =
  json => {
    let obj = Js.Json.decodeObject(json) |> unwrap;
    let package_count =
      Js.Dict.unsafeGet(obj, "package_count")
      |> Js.Json.decodeNumber
      |> unwrap
      |> int_of_float;
    let packages =
      Js.Dict.unsafeGet(obj, "packages")
      |> Js.Json.decodeArray
      |> unwrap
      |> Array.to_list
      |> List.map(Js.Json.decodeObject)
      |> List.map(unwrap)
      |> List.map(json_pkg =>
           Package.{
             name:
               Js.Dict.unsafeGet(json_pkg, "name")
               |> Js.Json.decodeString
               |> unwrap,
             description:
               Js.Dict.unsafeGet(json_pkg, "description")
               |> Js.Json.decodeString,
             /* TODO(@ostera): parse these 2 fields */
             versions: [],
             keywords: [],
             /* what have I done */
             target:
               (
                 Js.Dict.unsafeGet(json_pkg, "target")
                 |> Js.Json.decodeArray
                 |> unwrap
               )[0]
               |> Js.Json.decodeString
               |> (x => Belt.Option.getWithDefault(x, "unknown"))
               |> Js.String.toLowerCase
               |> Package.Target.of_string,
           }
         );

    {package_count, packages};
  };

let load: unit => Repromise.rejectable(Model.Index.t, _) =
  () => {
    Fetch.fetch(
      "https://raw.githubusercontent.com/reuniverse/reuniverse/master/packages/index.json",
    )
    |> Js.Promise.then_(Fetch.Response.json)
    |> Repromise.Rejectable.fromJsPromise
    |> Repromise.Rejectable.map(parse);
  };
