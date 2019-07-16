open Cmdliner;

let run = () => {
  open Rresult;
  let res =
    Index_builder.load(~path=Index_builder.default_index_path)
    >>= Pkg_builder.build_all_packages;
  Cmd_timer.print_duration();
  switch (res) {
  | Ok () => 0
  | _ => 1
  };
};

let cmd = {
  let doc = "Build all the packages in the Reuniverse";
  let exits = Term.default_exits;
  let man = [
    `S(Manpage.s_description),
    `P(
      {j|Reuniverse will read the index file generated with build-index and
         will attempt to build all the projects downloaded. |j},
    ),
  ];

  (
    Term.(const(run) $ Verbosity.arg),
    Term.info(
      "build-packages",
      ~doc,
      ~sdocs=Manpage.s_common_options,
      ~exits,
      ~man,
    ),
  );
};
