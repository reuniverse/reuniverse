/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Debug));
Logs.set_reporter(Logs_fmt.reporter());

// let get_pkgs = ["bsdoc", "reenv"] |> Npm.API.V2.packages;

let query =
  Npm.Search.Query.{params: [`Keywords(["reasonml", "bucklescript"])]};
let get_pkgs = Npm.API.V2.search(~query, ~from=0, ~size=100);

switch (get_pkgs |> Lwt_main.run) {
| exception e => Logs.err(m => m("%s", Printexc.to_string(e)))
| Ok(pkgs) =>
  pkgs.results
  |> List.map(pkg => pkg.Npm.Search.package)
  |> List.iter(pkg =>
       Npm.Package.(
         Logs.app(m => m("%s <%s>\n", pkg.name, pkg.repository.url))
       )
     )
| _ => ()
};
