module S = ReactDOMRe.Style;

let style = S.make(~padding="0", ());

[@react.component]
let make = (~packages) => {
  <ul style>
    {packages
     |> List.map(pkg => <Ds_package_card pkg />)
     |> Array.of_list
     |> React.array}
  </ul>;
};
