/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Debug));
Logs.set_reporter(Logs_fmt.reporter());

let index = Index_builder.build();

Logs.app(m => {
  let buffer = Buffer.create(1024);
  let fmt = Format.formatter_of_buffer(buffer);
  Index_builder.Index.pp(fmt, index);
  m("%s", buffer |> Buffer.contents);
});
