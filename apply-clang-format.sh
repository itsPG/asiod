#!/bin/bash

pushd "$(dirname "$0")" > /dev/null
find ./* -regex ".*\.cpp" -not -path "./build/*" -print0 | xargs -0 -P 8 clang-format -i -style=file
find ./* -regex ".*\.h" -not -path "./build/*" -print0 | xargs -0 -P 8 clang-format -i -style=file
popd > /dev/null