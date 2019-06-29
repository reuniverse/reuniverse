module Test = {
  [@react.component]
  let make = () => {
    <div>{React.string("Reuniverse")}</div>;
  };
};
