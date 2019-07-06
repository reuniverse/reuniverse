module S = ReactDOMRe.Style;

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
    ~background="radial-gradient(ellipse at bottom, #1b2735 0%, #090a0f 100%)",
    ~overflow="hidden",
    (),
  );

[@react.component]
let make = () => {
  <div style />;
};
