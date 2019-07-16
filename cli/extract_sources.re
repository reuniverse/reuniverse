open Cmdliner;

let run = () => {
  open Rresult;
  let res =
    Index_builder.load(~path=Index_builder.default_index_path)
    >>= Source_extractor.extract_all_sources;
  Cmd_timer.print_duration();
  switch (res) {
  | Ok () => 0
  | _ => 1
  };
};

let cmd = {
  let doc = "Extract package sources";
  let exits = Term.default_exits;
  let man = [
    `S(Manpage.s_description),
    `P(
      {j|Reuniverse will read the index file generated with build-index and
           will attempt to extract package sources already downloaded via
           pkg-download.  |j},
    ),
  ];

  (
    Term.(const(run) $ Verbosity.arg),
    Term.info(
      "extract-sources",
      ~doc,
      ~sdocs=Manpage.s_common_options,
      ~exits,
      ~man,
    ),
  );
};
