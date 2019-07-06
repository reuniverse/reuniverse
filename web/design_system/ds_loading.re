module S = ReactDOMRe.Style;

let style =
  S.make(~margin="2rem auto", ~width="200px", ~textAlign="center", ());

[@react.component]
let make = () => {
  <div style> {React.string("Loading...")} </div>;
};
