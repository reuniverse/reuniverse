# Informal Spec

1. Get a list of projects
1. Download them all to packages
1. For each package:
   1. Install dependencies with yarn
   2. Run `bsb -make-world`
   3. Install and run `bsdoc`
   4. Copy docs to `/docs/{package_name}`
1. For the list of packages generate an index in `/docs/index.html`
