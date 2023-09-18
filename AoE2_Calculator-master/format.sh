#!/bin/bash

### This script is used to run clang-format on the source files.

# Directory containing this bash script.
readonly DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

readonly PREV_DIR="$(pwd)"

cd "$DIR"
find ./ -name '*.h' -o -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
find ./ -name '*.h' -o -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
cd "$PREV_DIR"
exit 0

