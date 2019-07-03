# Contributing to reuniverse

Join our [Discord channel](https://discord.gg/QqpaWB2/)!

## Installing dependencies

### Install Node.js

To start working on reuniverse we need [Node.js](https://nodejs.org/). To install Node.js follow the instruction on their website. Node.js comes with a package manager called [npm](https://www.npmjs.com/).

### Install esy

We will use `npm` to install [`esy`](https://esy.sh/) globally on our machine:

```bash
npm install -g esy
```

esy is the package managment solution we use for writing reuniverse. esy itself uses [Dune](https://dune.build/) behind the scenes. We ourselves don't need to install Dune.

### Install fswatch

Dune needs `fswatch` to be able to monitor changes on our filesystem to use `esy start` command. Install [`fswatch`](https://github.com/emcrisostomo/fswatch/).

### Install Yarn

We will also need to install [Yarn](https://yarnpkg.com/) to build our Web interface. Follow the intructions on their [Installation page](https://yarnpkg.com/en/docs/install/).

## Building reuniverse

Clone the reuniverse repository and navigate to it:

```bash
git clone https://github.com/reuniverse/reuniverse.git
cd reuniverse
```

Install everything needed for reuniverse using esy:

```bash
esy
```

This will install everything we need for the project such as the OCaml compiler and other dependencies. This might take a couple of minutes the first time. Everything will be installed in a global cache on our machine which means that subsequent installations will be done in a matter of seconds, or even less.

Your filesystem should now look something like this:

```
├── .ci/
├── .git/
├── 3rdparty/
├── _esy/
├── cli/
├── esy.lock/
├── index_builder/
├── model/
├── node_modules/
├── npm/
├── packages/
├── utils/
├── web/
├── .gitignore
├── CONTRIBUTING.md
├── LICENSE
├── README.md
├── SPEC.md
├── azure-pipelines.yml
├── dune-project
├── esy.json
├── reuniverse.install
└── reuniverse.opam
```

If we compare this to the GitHub repository we notice that running `esy` has created a folder called `_esy`, `node_modules` and a file called `reuniverse.install`. This is our dependencies.

reuniverse is split up into two parts: the CLI and the Web interface

The CLI is located under `cli/` and the Web interface under `web/`. Dependencies for said project is specified in each `dune` file.

### Common dependencies

#### CLI

- [`Lwt` - OCaml promises and concurrent I/O](http://ocsigen.org/lwt/)
- [`Httpkit` - High-level, High-performance HTTP(S) Clients/Servers in Reason/OCaml](https://github.com/ostera/httpkit/)
- [`Logs` - Logging infrastructure for OCaml](https://erratique.ch/software/logs/)
- [`Fmt` - OCaml Format pretty-printer combinators](https://erratique.ch/software/fmt/)
- [`Cmdliner` - Declarative definition of command line interfaces for OCaml](https://erratique.ch/software/cmdliner/)

#### Web interface

- [`ReasonReact` - Reason bindings for ReactJS](https://reasonml.github.io/reason-react/)
- [`repromise` - Type-safe JS promises for ReasonML](https://github.com/aantron/repromise/)
- [`bs-fetch` - Fetch bindings for BuckleScript](https://github.com/reasonml-community/bs-fetch/)

### Building the CLI

Everything needed for the CLI is installed thanks to the `esy` command.

We should be able to run the CLI locally using `esy run`. If we need to pass in more parameters
you can pass them on with `esy run -- <params>`: `esy run -- --help`.

We could for example run the `index_builder` using the command:

```bash
esy run -- --build-index
```

For a build server, you can use: `esy start`.

### Building the Web interface

The Web project is located in the `web` folder and developed with BuckleScript. It relies on Yarn. If you
have Yarn installed, you can run: `cd web && yarn && yarn build` to get a complete build.

For a Reason build server, you can use: `yarn start`.

For a bundle, you can run `yarn build` again.

The site works as a static page with no other magic, so you can just open the
`index.html` from your favorite web browser.

### Troubleshooting

I can't run `esy start`

```
esy start
Error: fswatch (or inotifywait) was not found. One of them needs to be installed for watch mode to work.
```

Dune needs `fswatch` to be able to detect changes on the file system. Install [fswatch](https://github.com/emcrisostomo/fswatch/) and it should work.

I keep getting one of these errors:

```
reuniverse: internal error, uncaught exception:
            (Failure hd)
```

```
reuniverse: internal error, uncaught exception:
            Unix.Unix_error(Unix.EMFILE, "socket", "")
```

`Unix.EMFILE` refers to the fact that too many files are opened by the process (see: [Module Unix - Error report](https://caml.inria.fr/pub/docs/manual-ocaml/libref/Unix.html))
This is because our code is parallelized, and needs to create a lot of `file descriptors`. The amount of `file descriptors` your system can used is set using the `ulimit -n` command:

```bash
ulimit -n
256
```

256 is a bit low, we can set it to 24576 which seems to be the max on some systems:

```bash
ulimit -n 24576
```

Now you shouldn't be getting these errors!

For any other issues feel free to join our [Discord channel](https://discord.gg/QqpaWB2/)!
