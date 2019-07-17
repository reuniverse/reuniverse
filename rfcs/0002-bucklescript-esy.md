- Name: bucklescript-esy
- Start Date: 2019-07-09
- Issue/PR: (leave this empty)
- Point of Contact: ulrikstrid

# Summary

[summary]: #summary

Make Bucklescript builable with esy and make it Plug'n'Play compatible which fully unlocks esy.

# Motivation

[motivation]: #motivation

If we can get Bucklescript we can gather the community around 1 package manager and we might also be able to run basically any ppx by just depending on the correct OCaml compiler. We should also be able to create mono-repos with Bucklescript for the frontend code and native for backend.

# Guide-level explanation

[guide-level-explanation]: #guide-level-explanation

This change shouldn't impact the end-user anymore than that they can now use esy for package management.

# Reference-level explanation

[reference-level-explanation]: #reference-level-explanation

Initial exploration indicates that we need to change the build script (`./scripts/install.js`) so that it builds into a esy-compatible directory when built with `esy`.

We will also have to change how Buscklescript finds libraries slightly.

We might want to model the OCaml compiler as a esy dependency here as well but it's probably out of scope for this PR.

# Drawbacks

[drawbacks]: #drawbacks

Why should we _not_ do this?

# Rationale and alternatives

[alternatives]: #alternatives

If this is not done we can't use esy for Bucklescript and the divide between those that target native and js will still be huge.
We also unlock using Yarn pnp by doing this which seems like a huge win for disk space all around the world.

# Prior art

[prior-art]: #prior-art

There is a old implementation that does basically this here https://github.com/esy-ocaml/bucklescript.

# Unresolved questions

[unresolved]: #unresolved-questions

- If this doesn't get accepted upstream can we maintain a fork long term?
- Can we add esy/Yarn PnP support without building it with esy as well?
