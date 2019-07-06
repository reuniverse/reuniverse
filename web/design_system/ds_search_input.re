module S = ReactDOMRe.Style;

let style =
  S.combine(
    Ds_global.font_style,
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
