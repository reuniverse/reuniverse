/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Debug));
Logs.set_reporter(Logs_fmt.reporter());

let pkgs = Npm.API.V2.packages(["bsdoc", "reenv"]) |> Lwt_main.run;

switch (pkgs) {
| Ok(pkgs) =>
  pkgs
  |> List.map(pkg => pkg.Npm.Package_response.collected.metadata)
  |> List.iter(pkg =>
       Npm.Package.(
         Logs.app(m => m("%s <%s>\n", pkg.name, pkg.repository.url))
       )
     )
| _ => ()
};
