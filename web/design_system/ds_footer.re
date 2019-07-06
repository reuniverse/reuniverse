module S = ReactDOMRe.Style;

let style = S.make(~textAlign="center", ~height="50px", ());

[@react.component]
let make = () => {
  let built_with = {j|Built with ☕️ and ❤️ at|j};
  <Ds_container style>
    {built_with |> React.string}
    <Ds_link href="https://src.technology"> {"SRC" |> React.string} </Ds_link>
    <br />
    {"Index built from " |> React.string}
    <Ds_link
      href={
        "https://github.com/reuniverse/reuniverse/tree/" ++ Metadata.git_commit
      }>
      {Metadata.git_commit |> React.string}
    </Ds_link>
    {" at " ++ Metadata.build_time |> React.string}
  </Ds_container>;
};
