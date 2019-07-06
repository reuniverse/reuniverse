module S = ReactDOMRe.Style;

let style =
  S.make(
    ~background="#F9F9F9",
    ~padding="0.5rem",
    ~fontSize="10px",
    ~listStyle="none",
    ~color="black",
    (),
  );

let name_style =
  S.make(
    ~display="inline-block",
    ~width="25%",
    ~fontWeight="500",
    ~color="black",
    ~whiteSpace="nowrap",
    ~overflow="hidden",
    ~textOverflow="ellipsis",
    ~margin="0 1em",
    (),
  );

let desc_style =
  S.make(
    ~display="inline-block",
    ~width="60%",
    ~color="black",
    ~whiteSpace="nowrap",
    ~overflow="hidden",
    ~textOverflow="ellipsis",
    ~margin="0 1em",
    (),
  );

[@react.component]
let make = (~pkg) => {
  <li style>
    <span style=name_style> {Package.(pkg.name) |> React.string} </span>
    <span style=desc_style>
      {Utils.Infix.Option.(Package.(pkg.description) <|> "no description")
       |> React.string}
    </span>
    <Ds_label>
      Package.(pkg.target |> Target.to_string |> React.string)
    </Ds_label>
  </li>;
};
