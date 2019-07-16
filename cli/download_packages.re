open Cmdliner;

let run = () => {
  open Rresult;
  let res =
    Index_builder.load(~path=Index_builder.default_index_path)
    >>= Pkg_downloader.download_from_index;
  Cmd_timer.print_duration();
  switch (res) {
  | Ok () => 0
  | _ => 1
  };
};

let cmd = {
  let doc = "Download sources for all the packages in the Reuniverse";
  let exits = Term.default_exits;
  let man = [
    `S(Manpage.s_description),
    `P(
      {j|Reuniverse will read the index file generated with build-index and
           will attempt to download package sources for all the major versions
           of all the packages in the index.
           |j},
    ),
  ];

  (
    Term.(const(run) $ Verbosity.arg),
    Term.info(
      "download-packages",
      ~doc,
      ~sdocs=Manpage.s_common_options,
      ~exits,
      ~man,
    ),
  );
};
