module S = ReactDOMRe.Style;

let style = S.make(~padding="0", ~margin="0", ());

[@react.component]
let make = (~children) => <h5 style> children </h5>;
