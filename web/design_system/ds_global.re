module S = ReactDOMRe.Style;

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
