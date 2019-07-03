module Label = {
  let style =
    ReactDOMRe.Style.make(
      ~padding="5px",
      ~borderRadius="5px",
      ~fontSize="12px",
      ~fontFamily="Helvetica Neue, Helvetica, sans-serif",
      (),
    );

  [@react.component]
  let make = (~children) => <span style> children </span>;
};

module Search = {
  let style =
    ReactDOMRe.Style.make(
      ~width="100%",
      ~margin="0",
      ~padding="20px",
      ~border="0",
      ~outline="0",
      ~fontSize="24px",
      ~fontFamily="Helvetica Neue, Helvetica, sans-serif",
      (),
    );

  [@react.component]
  let make = (~onChange) => {
    <input
      style
      onChange={e => ReactEvent.Form.target(e)##value |> onChange}
      placeholder="Package name..."
    />;
  };
};

module Package = {
  let style =
    ReactDOMRe.Style.make(
      ~margin="10px",
      ~background="#F9F9F9",
      ~padding="20px",
      ~borderRadius="6px",
      ~listStyle="none",
      (),
    );

  [@react.component]
  let make = (~pkg) => {
    <li style>
      <b> {Package.(pkg.name) |> React.string} </b>
      <Label> Package.(pkg.target |> Target.to_string |> React.string) </Label>
      <br />
      {Utils.Infix.Option.(Package.(pkg.description) <|> "no description")
       |> React.string}
    </li>;
  };
};

module Pkg_list = {
  let style = ReactDOMRe.Style.make(~padding="0", ~width="70%", ());
  [@react.component]
  let make = (~index) => {
    let (filter, setFilter) = React.useState(() => "");
    Js.log(index);
    let pkgs =
      switch (filter) {
      | "" => Model.Index.(index.packages)
      | f =>
        Model.Index.(index.packages)
        |> List.filter(pkg => {
             let pattern = Js.Re.fromString(filter);
             Js.Re.test_(pattern, Model.Package.(pkg.name))
             || (
               switch (Model.Package.(pkg.description)) {
               | None => true
               | Some(desc) => Js.Re.test_(pattern, desc)
               }
             )
             || Js.Re.test_(
                  pattern,
                  Model.Package.(pkg.target |> Target.to_string),
                );
           })
      };
    <div>
      <Search onChange={text => setFilter(text)} />
      <ul style>
        {pkgs
         |> List.map(pkg => <Package pkg />)
         |> Array.of_list
         |> React.array}
      </ul>
    </div>;
  };
};
