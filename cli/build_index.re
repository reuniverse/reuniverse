open Cmdliner;

let run = () => {
  let res =
    Index_builder.build()
    |> Index_builder.save(~path=Index_builder.default_index_path);
  Cmd_timer.print_duration();
  switch (res) {
  | Ok () => 0
  | _ => 1
  };
};

let cmd = {
  let doc = "Build the Reason Package Index by scanning several sources.";
  let exits = Term.default_exits;
  let man = [
    `S(Manpage.s_description),
    `P(
      {j|Reuniverse will scan package sources, such as npm, to build a package
           index. This index will be written down to \$ROOT/packages/index.json.  |j},
    ),
  ];

  (
    Term.(const(run) $ Verbosity.arg),
    Term.info(
      "build-index",
      ~doc,
      ~sdocs=Manpage.s_common_options,
      ~exits,
      ~man,
    ),
  );
};
