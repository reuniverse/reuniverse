module Cmd = {
  let bsb = Bos.Cmd.(v("bsb"));

  let clean_world = () => {
    Bos.Cmd.(bsb % "-clean-world")
    |> Bos.OS.Cmd.run
    |> Rresult.R.reword_error((`Msg(e)) => `Bsb_clean_world_error(e));
  };

  let make_world = () => {
    Bos.Cmd.(bsb % "-make-world")
    |> Bos.OS.Cmd.run
    |> Rresult.R.reword_error((`Msg(e)) => `Bsb_make_world_error(e));
  };
};
