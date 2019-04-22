#!/bin/bash

REPOS=$(find . -type d -name "git" -maxdepth 2 | sort -u)

PKGS=$(find . -type d -name "git" -maxdepth 2 | awk -F'/' '{ print $2 }' | sort -u)

DOCS_DIR=$(pwd)/../docs

mkdir -p ${DOCS_DIR}

for repo in ${REPOS}; do

  PROJECT_NAME=$(cat ${repo}/package.json | jq -r ".name" | sed "s/-/_/g")

  echo "====================" ${PROJECT_NAME} "===================="

  pushd $repo;

    rm -rf ./docs

    rm -rf ./node_modules

    yarn add bs-platform@6.0.0-dev.1 bsdoc@6.0.2-alpha

    yarn

    yarn run bsb -make-world

    yarn run bsdoc build ${PROJECT_NAME}

    cp -R ./docs/* ${DOCS_DIR}/

    echo "."
    echo "."

  popd;

done
