- Name: bootstrap_rfc
- Start Date: 2019-06-03
- Issue/PR: (leave this empty)
- Point of Contact: Leandro Ostera <leandro@src.technology>

# Summary
[summary]: #summary

Reuniverse is a _universal documentation hub for the entire Reason ecosystem_,
becoming the one place to find packages, and read package documentation and
guides.

It intends to address the problems of:

* Package discovery
* Documentation discovery and publishing

And as a by product, build a platform that we can hand over to the community,
on top of which to centralize the Reason ecosystem.

# Motivation
[motivation]: #motivation

Getting started with Reason, both for web development and for native
development, has a huge cognitive load.

We have a fragmented ecosystem with 2 compilers, 2 build systems, several
standard libraries, and no authoritative place to learn how all these pieces
fit together.

If you want to learn about the language, you have to go to
[reasonml.github.io](https://reasonml.github.io).

If you want to learn about the compiler, you have to go to
[bucklescript.github.io](https://bucklescript.github.io).

If you want to learn about the standard libraries, you have a few options:

* [OCaml 4.02 Stdlib at
  Inria](https://caml.inria.fr/pub/docs/manual-ocaml-4.02/stdlib.html)
* [Reason-syntax version of the OCaml 4.02
  Stdlib](https://reasonml.github.io/api/index.html)
* [BuckleScript Stdlib](https://bucklescript.github.io/bucklescript/api/) 

And this list doesn't include community developed standard libraries such as:

* [Relude](https://reazen.github.io/relude/#/)
* [Tablecloth](https://github.com/darklang/tablecloth/blob/master/bs/src/tablecloth.mli)
* [Rationale](https://github.com/jonlaing/rationale)

Further more, every package that is made available has to build their own
documentation infrastructure. Many popular ones recurring to linking to their
top-level interface file, like
[`bs-css`](https://github.com/SentiaAnalytics/bs-css/blob/master/src/Css.rei). 

High-level soft-documentation is also hard to find in good quality and
up-to-date, with projects such as [Awesome
ReasonML](https://github.com/vramana/awesome-reasonml), [Reason Design
Patterns](https://github.com/ostera/reason-design-patterns), and the [Weekly
Reason Newsletter](http://www.reasonml.online), and several individual blogs as
the better examples out there, and quickly falling behind with language,
library, and compiler updates.

And all of this is assuming that you already know what libraries you are
looking for. Package discovery was attempted to be solved by
[Redex](https://redex.github.io) but its manual nature and it being developed
and maintained by a single individual make it unscalable.

It is this that motivates the creation of Reuniverse, as a community governed 
documentation hub.

# Guide-level explanation
[guide-level-explanation]: #guide-level-explanation

I'd like to explain this project from 2 perspectives: the perspective of a
library author, and the perspective of a library user.

### I'm a Library Author

Say you are a library author, and you're building a new library, or writing
bindings to an external one. 

You wrote some documentation in your interface files, and you have some example
code as well. Your source code is on Github, and you have made your repository
public.

Within the next hour, your package will be indexed by Reuniverse. After
indexing, the `HEAD` and every tagged version (`v0.1.0`, etc) will be built,
and documentation for every version will be generated and published.

If you published your package to npm, or the Github package registry,
Reuniverse will make note of that and use that as a source of information as
well to show library users how to install your library.

### I'm a Library User

So you are looking for a library. You got some keywords in mind, like `reason`
and `css` and `classnames`. You go into Reuniverse, and in the top-level search
bar you type in `reason css classnames`.

You get a few hits: `bs-css`, `bs-classnames`. Ranked by how close the match
was.

You can quickly see when the last release was made, and what the last release
version is. With a click you open up the documentation for `bs-css` to see what
the library looks like.

Inside, you can see all the symbols exported by the library, detailed metadata
on the version, what compiler version it's built for, and any guides written
for the library.

You can also search within the body of that documentation, 

You like what you see, and copy the command to install it: `yarn
add bs-css`.

You move on to building good software.

---

That is really all you have to do to publish and find libraries and
documentation for Reason.

# Reference-level explanation
[reference-level-explanation]: #reference-level-explanation

Reuniverse works as a data pipeline that transforms information, decorates it,
and uses it to produce a series of artifacts.

In its current form it works on stages:

1. Building the Index:
   * Reuniverse queries npm, gathering a wide list of packages with specific
     keywords (such as `bucklescript`, or `reason`).
   * Decorates these packages with information from their sources, using
     unpkg.com, to tag them as targeting the Web, Native, Universal, or Unknown
     for fallback.
   * Writing down an package index in JSON format that we can use for different
     feature work.

2. Generating Documentation:
   * Reading from the package index, it downloads the sources for all the
     published versions of the packages in the index
   * Generates documentation for all the packages in it index, using `bsdoc`
     for web packages, and `odoc` for native and universal ones.

3. Building the Website:
   * Currently this builds a Javascript web application, bundled with Fastpack,
     that will load the index on runtime.

4. Publishing:
   * Both the website, the index, and the documentation files are published to
     a Github repository: github.com/reuniverse/reuniverse.github.io, for public
     usage.

Going forward, Reuniverse can be split into 3 core concerns: data, features,
and infrastructure. And I'd like us to keep them as tightly coupled as they
need to be, but not any more than that, to allow for a diverse group of people
to collaborate from their area of expertise into making features that touch all
concerns in a smooth fashion.

## Data Concerns

1. **Building a package index**, by scanning sources such as the npm inc.
   registry, the github package registry, github repositories, and other
   sources of information (such as manually submitted packages), we will build
   an index of reason packages.

2. **Refining the package index**, by using other sources such as unpkg.com to
   quickly verify assumptions on the packages. For example, we have tagged each
   package with a _target platform_ (`Native | Web | Universal | Unknown`) by
   quickly looking into unpkg.com for specific files.

3. **Creating index projections**, for different features. This is important
   because the main index, with all the package metadata, can grow quite big,
   quite fast. Different features require different transformations of that
   index, and we should allow for arbitrary steps to be run to generate
   projections of data for search purposes, for visualization, for detail
   pages, and more.

All of these would run periodically, updating the index and projections with
fresh data for the features to use. It is paramount to get the _infrastructure_
right for this to work.

## Features

The _features_ are the _value_ that reuniverse gives developers. The initial
set of features we would like to deliver are:

0. **Package Index** - reuniverse should provide an authoritative index of
   packages included annotated metadata from different sources, that can be
   used to power all the following features, and any other tooling around it.
   This means that if there is a use-case that requires more information for
   packages in the index, the index should be extended to include it.

1. **Package Search** — reuniverse should be equipped with is a fast package
   search for the whole of the ecosystem. This has [already been
   done](https://redex.github.io), and lessons can be learned from it.

2. **Package Documentation** — reuniverese should handle the generation of
   docs for all the packages in the index, for all the versions that we are
   aware of, and cross-linking whenever possible.
   
Additional features we want to have are:

3. **Type-level Search** — reuniverse should allow you to write a type-signature
   and find the symbols that are related to it, across packages,  and provide
   access to the documentation of those symbols. 

4. **Documentation Search** — reuniverse should allow you to search across all
   the docs for all the packages.

5. **Playground** — reuniverse should provide a playground where you can mesh up
   libraries and try them out, provided they are runnable as web projects. This
   ties up closely with work being done by @thangngoc89 on Sketch.sh, and is a
   point where we should collaborate.

And many others such as Getting Started guides, bringing into it the Reason
Weekly newsletter, and other soft documentation like common design patterns.

All of these we can work on separately and over time.

## Infrastructure

Last but absolutely not least, we have our _infrastructure_ concerns:

1. Design System — defining a cohesive visual experience across the entire
   website, including future hand-crafted user-flows for onboarding users into
   the platform.

2. CI pipelines — at the very bottom of our work there is the foundational
   pipelining keeps Reuniverse alive. It runs data jobs periodically, builds
   the feature projects, tests shared libraries, and publishes the indexes and
   the website.

---

With this said. It is my hope that we can align as a community to make
Reuniverse a reality beyond the efforts that [SRC](https://src.technology) is
pouring into it.

# Drawbacks
[drawbacks]: #drawbacks

Some of the problems this has are:

* Package scanning is _hard_. If we don't standardize on how to tag packages to
  ease the discovery, we will risk missing packages that were not picked up on
  the broad querying to the package registries. 

* Infrastructure isn't free. Right now we are running Azure DevOps Pipelines,
  but the allocated times to run these jobs will diminish as the jobs grow
  longer, and dedicated infrastructure to execute reuniverse may be required.

# Rationale and alternatives
[alternatives]: #alternatives

We could instead go down the path of Redex and manually maintain an index of
packages, but the work involved in maintaining this index and documentation hub
up to date has proven to be a barrier for other ecosystems in the past (such as
Opam).

In the future, I'd like to see a `reuniverse` CLI tool to make it trivial to
register a package in the index without scanning being necessary.

# Prior art
[prior-art]: #prior-art

The Rust language community has developed a documentation site:
[`docs.rs`](https://docs.rs) that is based on similar principles.

A core difference we have is that `docs.rs` is powered by the [crates package
registry](https://crates.io), which contains the authoritative metadata of all
the packages in their ecosystem.

The onboarding that is possible due to the unified ecosystem in Rust, makes it
possible to have Getting Started guides that are [incredibly straightforward to
follow](https://www.rust-lang.org/learn/get-started).

# Unresolved questions
[unresolved]: #unresolved-questions

As we scale, we will need to pay more attention to:

* Hosting of metadata, compilation artifacts, and website
* Governance
