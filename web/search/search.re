module State = {
  type t = {index: option(Model.Index.t)};

  let make = (~index) => {index: Some(index)};

  let initial = {index: None};
};

module Effects = {
  let load_index = (state, setState) => {
    React.useEffect(() => {
      switch (State.(state.index)) {
      | None =>
        Pkg_index_data.load()
        |> Repromise.Rejectable.map(index => {
             setState(_ => State.make(~index));
             Repromise.Rejectable.resolved();
           })
        |> ignore
      | Some(_) => ()
      };
      None;
    });
  };
};

let match_name = (pattern, pkg) =>
  Js.Re.test_(pattern, Model.Package.(pkg.name));
let match_desc = (pattern, pkg) =>
  switch (Model.Package.(pkg.description)) {
  | None => true
  | Some(desc) => Js.Re.test_(pattern, desc)
  };
let match_target = (pattern, pkg) =>
  Js.Re.test_(pattern, Model.Package.(pkg.target |> Target.to_string));

let find_pkgs = (~filter) =>
  List.filter(pkg => {
    let pattern = Js.Re.fromString(filter);
    match_name(pattern, pkg)
    || match_desc(pattern, pkg)
    || match_target(pattern, pkg);
  });

let handle_results = (~filter, index) => {
  let pkgs = Model.Index.(index.packages);
  let pkgs =
    switch (filter) {
    | "" => pkgs
    | _ => find_pkgs(~filter, pkgs)
    };

  let pkg_count = pkgs |> List.length |> string_of_int;

  let heading =
    switch (filter) {
    | "" =>
      <Design_system.Tiny_title>
        {pkg_count ++ " packages in the ecosystem." |> React.string}
      </Design_system.Tiny_title>
    | _ =>
      <Design_system.Tiny_title>
        {pkg_count
         ++ " search results for \""
         ++ filter
         ++ "\""
         |> React.string}
      </Design_system.Tiny_title>
    };

  Design_system.(
    <Container> heading <Package_list packages=pkgs /> </Container>
  );
};

module App = {
  [@react.component]
  let make = (~filter) => {
    let (state, setState) = React.useState(() => State.initial);

    Effects.load_index(state, setState);

    switch (state.index) {
    | None => <Design_system.Loading />
    | Some(index) => handle_results(~filter, index)
    };
  };
};
