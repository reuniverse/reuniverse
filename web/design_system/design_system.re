module S = ReactDOMRe.Style;

module Global = {
  let font_style =
    S.make(
      ~fontSize="1rem",
      ~fontFamily="Montserrat, Helvetica Neue, Helvetica, sans-serif",
      ~color="white",
      (),
    );
  let style = S.combine(font_style, S.make());
  [@react.component]
  let make = (~children) => {
    <section style> children </section>;
  };
};

module Container = {
  let default_style = S.make(~margin="0", ~padding="1rem", ());
  [@react.component]
  let make = (~style=S.make(), ~children) => {
    <section style=S.combine(default_style, style)> children </section>;
  };
};

module Label = {
  let style =
    S.make(
      ~padding="5px",
      ~backgroundColor="#232323",
      ~borderRadius="5px",
      ~minWidth="50px",
      ~textTransform="uppercase",
      ~color="white",
      ~fontSize="1em",
      ~fontWeight="500",
      ~fontFamily="Helvetica Neue, Helvetica, sans-serif",
      (),
    );

  [@react.component]
  let make = (~children) => <span style> children </span>;
};


module Link = {
  let style = S.make(~color="white", ~textDecoration="none", ());
  [@react.component]
  let make = (~href, ~children) => {
    <Label> <a style href> children </a> </Label>
  };
};

module Footer = {
  let style = S.make(~textAlign="center", ~height="50px", ());

  [@react.component]
  let make = () => {
    let built_with = {j|Built with ☕️ and ❤️ at|j};
    <Container style>
      {built_with |> React.string}
      <Link href="https://src.technology"> {"SRC" |> React.string} </Link>
      <br />
    </Container>;
  };
};

module Background = {
  let style =
    S.make(
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
    S.make(~margin="2rem auto", ~width="200px", ~textAlign="center", ());
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
    S.make(~maxWidth="80px", ~padding="1rem", ~display="inline-block", ());

  [@react.component]
  let make = () => {
    <img style src="./assets/logo.svg" />;
  };
};

module Search_input = {
  let style =
    S.combine(
      Global.font_style,
      S.make(
        ~width="200px",
        ~margin="0",
        ~padding="1rem",
        ~border="0",
        ~outline="0",
        ~float="right",
        ~background="none",
        (),
      ),
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

module Tiny_title = {
  let style = S.make(~padding="0", ~margin="0", ());
  [@react.component]
  let make = (~children) => <h5 style> children </h5>;
};

module Package_card = {
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
      <Label> Package.(pkg.target |> Target.to_string |> React.string) </Label>
    </li>;
  };
};

module Package_list = {
  let style = S.make(~padding="0", ());
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
