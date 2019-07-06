module S = ReactDOMRe.Style;

let style =
  S.make(~maxWidth="80px", ~padding="1rem", ~display="inline-block", ());

[@react.component]
let make = () => {
  <img style src="./assets/logo.svg" />;
};
