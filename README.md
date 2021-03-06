# re:universe

[![Build Status](https://dev.azure.com/reuniverse/reuniverse/_apis/build/status/reuniverse.reuniverse?branchName=master)](https://dev.azure.com/reuniverse/reuniverse/_build/latest?definitionId=1?branchName=master)

Package documentation for the entire Reason ecosystem. Coming soon!

## Contributing

Join our [Discord channel](https://discord.gg/QqpaWB2/)!

### Building the CLI

The CLI project is developed as a native program, and it relies on esy and Dune.
If you have esy installed, it should be a matter of running: `esy` to get a
complete build in place.

For a build server, you can use: `esy start`.

Locally, you can run the CLI with `esy run`. If you need to pass in more parameters
you can pass them on with `esy run -- <params>`: `esy run -- --help`.

### Building the Web interface

The Web project is located in the `web` folder and developed with BuckleScript. It relies on Yarn. If you
have Yarn installed, you can run: `cd web && yarn && yarn build` to get a complete build.

For a Reason build server, you can use: `yarn start`.

For a bundle, you can run `yarn build` again.

The site works as a static page with no other magic, so you can just open the
`index.html` from your favorite web browser.

## In-depth instructions

See [CONTRIBUTING](https://github.com/reuniverse/reuniverse/blob/master/CONTRIBUTING.md/)
