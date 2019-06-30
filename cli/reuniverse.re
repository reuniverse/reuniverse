/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Info));
Logs.set_reporter(Logs_fmt.reporter());

let index = Index_builder.build();

Index_builder.save(index);

Logs.app(m => {
  let buffer = Buffer.create(1024);
  let fmt = Format.formatter_of_buffer(buffer);
  Model.Index.pp(fmt, index);
  m("%s", buffer |> Buffer.contents);
});
