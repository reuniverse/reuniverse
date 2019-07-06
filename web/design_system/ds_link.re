module S = ReactDOMRe.Style;

let style = S.make(~color="white", ~textDecoration="none", ());

[@react.component]
let make = (~href, ~children) => {
  <Ds_label> <a style href> children </a> </Ds_label>;
};
