let global_style =
  ReactDOMRe.Style.make(
    ~fontSize="1rem",
    ~fontFamily="Montserrat, Helvetica Neue, Helvetica, sans-serif",
    ~color="white",
    (),
  );

module Container = {
  let style = ReactDOMRe.Style.make(~margin="0", ~padding="1rem", ());
  [@react.component]
  let make = (~children) => {
    <section style> children </section>;
  };
};

module Background = {
  let style =
    ReactDOMRe.Style.make(
      ~position="fixed",
      ~top="0",
      ~left="0",
      ~right="0",
      ~bottom="0",
      ~height="100%",
      ~width="100%",
      ~zIndex="-1",
      ~background=
        "radial-gradient(ellipse at bottom, #1b2735 0%, #090a0f 100%)",
      ~overflow="hidden",
      (),
    );
  [@react.component]
  let make = () => {
    <div style />;
  };
};

module Loading = {
  let style =
    ReactDOMRe.Style.make(
      ~margin="2rem auto",
      ~width="200px",
      ~textAlign="center",
      (),
    );
  [@react.component]
  let make = () => {
    <div style> {React.string("Loading...")} </div>;
  };
};

module Nav = {
  [@react.component]
  let make = (~children) => {
    <nav> children </nav>;
  };
};

module Logo = {
  let style =
    ReactDOMRe.Style.make(
      ~maxWidth="80px",
      ~padding="1rem",
      ~display="inline-block",
      (),
    );

  [@react.component]
  let make = () => {
    <img style src="./assets/logo.svg" />;
  };
};

module Label = {
  let style =
    ReactDOMRe.Style.make(
      ~padding="5px",
      ~borderRadius="5px",
      ~minWidth="50px",
      ~fontSize="8px",
      ~fontFamily="Helvetica Neue, Helvetica, sans-serif",
      (),
    );

  [@react.component]
  let make = (~children) => <span style> children </span>;
};

module Search_input = {
  let style =
    ReactDOMRe.Style.make(
      ~fontSize="1rem",
      ~width="200px",
      ~margin="0",
      ~padding="1rem",
      ~border="0",
      ~outline="0",
      ~float="right",
      ~color="white",
      ~background="none",
      (),
    );

  [@react.component]
  let make = (~onChange, ~placeholder) => {
    <input
      style
      onChange={e => ReactEvent.Form.target(e)##value |> onChange}
      placeholder
    />;
  };
};

module Subtitle = {
  [@react.component]
  let make = (~children) => <h4> children </h4>;
};

module Package_card = {
  let style =
    ReactDOMRe.Style.make(
      ~background="#F9F9F9",
      ~padding="0.2rem 0.5rem",
      ~fontSize="10px",
      ~listStyle="none",
      ~color="black",
      (),
    );

  let name_style =
    ReactDOMRe.Style.make(
      ~display="inline-block",
      ~width="150px",
      ~fontSize="10px",
      ~fontWeight="500",
      ~color="black",
      ~whiteSpace="nowrap",
      ~overflow="hidden",
      ~textOverflow="ellipsis",
      (),
    );

  let desc_style =
    ReactDOMRe.Style.make(
      ~display="inline-block",
      ~width="300px",
      ~fontSize="10px",
      ~color="black",
      ~whiteSpace="nowrap",
      ~overflow="hidden",
      ~textOverflow="ellipsis",
      (),
    );

  [@react.component]
  let make = (~pkg) => {
    <li style>
      <Label> Package.(pkg.target |> Target.to_string |> React.string) </Label>
      <span style=name_style> {Package.(pkg.name) |> React.string} </span>
      <span style=desc_style> {Utils.Infix.Option.(Package.(pkg.description) <|> "no description")
       |> React.string}
       </span>
    </li>;
  };
};

module Package_list = {
  let style = ReactDOMRe.Style.make(~padding="0", ());
  [@react.component]
  let make = (~packages) => {
    <ul style>
      {packages
       |> List.map(pkg => <Package_card pkg />)
       |> Array.of_list
       |> React.array}
    </ul>;
  };
};
