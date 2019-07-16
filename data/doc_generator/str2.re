let to_camel_case = str => {
  let chars = str |> String.capitalize_ascii |> String.to_seq |> List.of_seq;
  let rec up = (acc, cs) => {
    switch (cs) {
    | ['-', x, ...xs] => up([x |> Char.uppercase_ascii, ...acc], xs)
    | [x, ...xs] => up([x, ...acc], xs)
    | [] => acc |> List.rev |> List.to_seq |> String.of_seq
    };
  };
  up([], chars);
};
