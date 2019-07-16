let began_at = Unix.gettimeofday();
let print_duration = () => {
  let ended_at = Unix.gettimeofday();
  Logs.app(m => m("Finished in %.2fs", ended_at -. began_at));
};
