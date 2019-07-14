/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Info));
Logs.set_reporter(Logs_fmt.reporter());

open Cmdliner;

module Cmd_timer = {
  let began_at = Unix.gettimeofday();
  let print_duration = () => {
    let ended_at = Unix.gettimeofday();
    Logs.app(m => m("Finished in %.2fs", ended_at -. began_at));
  };
};

module Verbosity = {
  let setup = level => {
    Fmt_tty.setup_std_outputs();
    Logs.set_level(level);
    Logs.set_reporter(Logs_fmt.reporter());
  };

  let arg = Term.(const(setup) $ Logs_cli.level());
};

module Build_index = {
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
};

module Pkg_download = {
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
    let doc = "Download package sources";
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
        "pkg-download",
        ~doc,
        ~sdocs=Manpage.s_common_options,
        ~exits,
        ~man,
      ),
    );
  };
};

let default_cmd = {
  let doc = "A Composable Static Site Generator";
  let sdocs = Manpage.s_common_options;
  let exits = Term.default_exits;
  (
    Term.(ret(const(`Help((`Pager, None))))),
    Term.info("reuniverse", ~version="%%VERSION%%", ~doc, ~sdocs, ~exits),
  );
};

let cmds = [Build_index.cmd, Pkg_download.cmd];

Term.(exit @@ eval_choice(default_cmd, cmds));
