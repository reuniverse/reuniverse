module Option = {
  let (<|>) = (o, d) => switch(o) { | None => d | Some(x) => x };
};
