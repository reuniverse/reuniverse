open Cmdliner;

let setup = level => {
  Fmt_tty.setup_std_outputs();
  Logs.set_level(level);
  Logs.set_reporter(Logs_fmt.reporter());
};

let arg = Term.(const(setup) $ Logs_cli.level());
