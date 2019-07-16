module Package_json = {
  module Dependency_list = {
    type t = list((string, string));

    let to_yojson: t => Yojson.Safe.t =
      pkgs => `Assoc(List.map(((k, v)) => (k, `String(v)), pkgs));

    let of_yojson: Yojson.Safe.t => Ppx_deriving_yojson_runtime.error_or(t) =
      _t => {
        Error("not implemented.");
      };

    let make = pkgs => pkgs;
  };

  [@deriving yojson]
  type t = {
    name: string,
    private: bool,
    workspaces: list(string),
    dependencies: Dependency_list.t,
  };

  let make = (~private=true, ~workspaces=[], ~dependencies=[], name) => {
    {name, private, workspaces, dependencies};
  };
};

module Cmd = {
  let yarn = (~force=false, ~ignore_scripts=false, ~cache_folder) =>
    Bos.Cmd.(
      v("yarn")
      %% (force ? Bos.Cmd.v("--force") : Bos.Cmd.empty)
      % "--non-interactive"
      %% (ignore_scripts ? Bos.Cmd.v("--ignore_scripts") : Bos.Cmd.empty)
      % "--cache-folder"
      % (cache_folder |> Fpath.to_string)
    );

  let install = (~force=false, ~ignore_scripts=false, ~cache_folder, ()) => {
    Bos.Cmd.(yarn(~force, ~ignore_scripts, ~cache_folder) % "install")
    |> Bos.OS.Cmd.run
    |> Rresult.R.reword_error((`Msg(e)) => `Yarn_install_error(e));
  };

  let add =
      (
        ~force=false,
        ~global=false,
        ~ignore_scripts=false,
        ~cache_folder,
        ~save=`None,
        deps,
      ) => {
    let global =
      switch (global) {
      | true => Bos.Cmd.v("global")
      | _ => Bos.Cmd.empty
      };
    let save =
      switch (save) {
      | `None => Bos.Cmd.empty
      | `Deps => Bos.Cmd.v("--save")
      | `Dev => Bos.Cmd.v("--dev")
      };
    let deps =
      Bos.Cmd.(
        deps
        |> List.fold_left(
             (acc, (name, ver)) => v(name ++ "@" ++ ver) %% acc,
             empty,
           )
      );
    Bos.Cmd.(
      yarn(~force, ~ignore_scripts, ~cache_folder)
      %% global
      % "add"
      %% deps
      %% save
    )
    |> Bos.OS.Cmd.run
    |> Rresult.R.reword_error((`Msg(e)) => `Yarn_add_error(e));
  };
};
