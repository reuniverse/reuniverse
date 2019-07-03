# What is reuniverse?

reuniverse is:

- an index of Reason packages
- docs for all packages
- docs for the lang and compiler

## Package Index

reuniverse is an index of Reason packages, collected and scrapped from NPM and
from the Github Package Registry.

It works by periodically pulling those registries for packages that match a
certain criteria, filtering those packages, and publishing the index for further
processing.

## Package Documentation

reuniverse is a portal with documentation from all packages that made it to the
index.

It includes documentation for all published package versions that were compilable
with BuckleScript 6.

That is, if package `bs-css` has 10 tags, from `v0.1.0` to `v0.10.0`, and only
`v0.8.0` through `v0.10.0` are compilable with BuckleScript 6, there will be
documentation available for those 3 versions. Provided the documentation compiles
successfully.

## Language and Compiler Documentation

reuniverse is a documentation portal with docs on the language and the compiler.
