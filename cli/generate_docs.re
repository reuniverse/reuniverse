open Cmdliner;

let run = () => {
  open Rresult;
  let res =
    Index_builder.load(~path=Index_builder.default_index_path)
    >>= Doc_generator.generate_docs;
  Cmd_timer.print_duration();
  switch (res) {
  | Ok () => 0
  | _ => 1
  };
};

let cmd = {
  let doc = "Generate docs for built packages";
  let exits = Term.default_exits;
  let man = [
    `S(Manpage.s_description),
    `P(
      {j|Reuniverse will generate documentation for all the packages that have
         been built with build-packages. |j},
    ),
  ];

  (
    Term.(const(run) $ Verbosity.arg),
    Term.info(
      "gen-docs",
      ~doc,
      ~sdocs=Manpage.s_common_options,
      ~exits,
      ~man,
    ),
  );
};
