/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Info));
Logs.set_reporter(Logs_fmt.reporter());

open Cmdliner;

let default_cmd = {
  let doc = "A Universal Documentation Hub for Reason";
  let sdocs = Manpage.s_common_options;
  let exits = Term.default_exits;
  (
    Term.(ret(const(`Help((`Pager, None))))),
    Term.info("reuniverse", ~version="%%VERSION%%", ~doc, ~sdocs, ~exits),
  );
};

let cmds = [
  Init_reuniverse.cmd,
  Build_index.cmd,
  Download_packages.cmd,
  Extract_sources.cmd,
  Build_packages.cmd,
  Generate_docs.cmd,
];

Term.(exit @@ eval_choice(default_cmd, cmds));
