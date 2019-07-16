open Cmdliner;
open Rresult;

let run = () => {
  let res =
    Index_builder.build()
    |> Index_builder.save(~path=Index_builder.default_index_path)
    >>= (() => Index_builder.load(~path=Index_builder.default_index_path))
    >>= (
      index =>
        Pkg_downloader.download_from_index(index)
        >>= (() => Source_extractor.extract_all_sources(index))
        >>= (() => Pkg_builder.build_all_packages(index))
        >>= (() => Doc_generator.generate_docs(index))
    );
  Cmd_timer.print_duration();
  switch (res) {
  | Ok () => 0
  | _ => 1
  };
};

let cmd = {
  let doc = "Bootstrap Reuniverse";
  let exits = Term.default_exits;
  let man = [`S(Manpage.s_description)];

  (
    Term.(const(run) $ Verbosity.arg),
    Term.info("init", ~doc, ~sdocs=Manpage.s_common_options, ~exits, ~man),
  );
};
