module Cmd = {
  let bsdoc = Bos.Cmd.(v("bsdoc"));

  let build = (~output_dir, name) => {
    Bos.Cmd.(
      bsdoc % "build" % "--output" % (output_dir |> Fpath.to_string) % name
    )
    |> Bos.OS.Cmd.run
    |> Rresult.R.reword_error((`Msg(e)) => `Bsdoc_gen_error(e));
  };
};
