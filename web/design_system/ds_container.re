module S = ReactDOMRe.Style;

let default_style = S.make(~margin="0", ~padding="1rem", ());

[@react.component]
let make = (~style=S.make(), ~children) => {
  <section style={S.combine(default_style, style)}> children </section>;
};
