module S = ReactDOMRe.Style;

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
