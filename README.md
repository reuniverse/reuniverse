# re:universe

[![Build Status](https://dev.azure.com/reuniverse/reuniverse/_apis/build/status/reuniverse.hello-reason?branchName=master)](https://dev.azure.com/reuniverse/reuniverse/_build/latest?definitionId=1?branchName=master)
[![Board Status](https://dev.azure.com/reuniverse/ad9592cf-fadf-4ddc-ab19-a378af5f488b/cae4c130-c38d-447c-8ab1-d96c8b80ec3b/_apis/work/boardbadge/7046e256-f990-423a-b90f-19d89081aa44)](https://dev.azure.com/reuniverse/ad9592cf-fadf-4ddc-ab19-a378af5f488b/_boards/board/t/cae4c130-c38d-447c-8ab1-d96c8b80ec3b/Microsoft.RequirementCategory)

## Contributing 

### Building the CLI

The CLI project is developed as a native program, and it relies on esy and dune.
If you have esy installed, it should be a matter of running: `esy` to get a 
complete build in place.

For a build server, you can use: `esy start`.

Running the build server requires [`fswatch`](https://github.com/emcrisostomo/fswatch).

Locally, you can run the cli with `esy run`. If you need to pass in more parameters
you can pass them on with `esy run -- <params>`: `esy run -- --help`.

### Building the Web interface

The Web project is developed with BuckleScript, and it relies on yarn. If you 
have yarn installed, you can run: `yarn && yarn build` to get a complete build.

For a Reason build server, you can use: `yarn start`.

For a bundle, you can run `yarn build` again.

The site works as a static page with no other magic, so you can just open the
`index.html` from your favorite web browser.
