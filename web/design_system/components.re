module Search = {
  [@react.component]
  let make = (~onChange) => {
    <input
      onChange={e => ReactEvent.Form.target(e)##value |> onChange}
      placeholder="Package name..."
    />;
  };
};

module Pkg_list = {
  [@react.component]
  let make = (~index) => {
    let (filter, setFilter) = React.useState(() => "");
    Js.log(index);
    let pkgs =
      switch (filter) {
      | "" => Index.(index.packages)
      | f =>
        Index.(index.packages)
        |> List.filter(pkg => {
             let pattern = Js.Re.fromString(filter);
             Js.Re.test_(pattern, Package.(pkg.name))
             || (
               switch (Package.(pkg.description)) {
               | None => true
               | Some(desc) => Js.Re.test_(pattern, desc)
               }
             );
           })
      };
    <div>
      <Search onChange={text => setFilter(text)} />
      <ul>
        {pkgs
         |> List.map(pkg =>
              <li>
                <b> {Package.(pkg.name) |> React.string} </b>
                {"-" |> React.string}
                {Utils.Infix.Option.(
                   Package.(pkg.description) <|> "no description"
                 )
                 |> React.string}
              </li>
            )
         |> Array.of_list
         |> React.array}
      </ul>
    </div>;
  };
};
