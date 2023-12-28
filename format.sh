#!/bin/bash

### This script is used to run clang-format on the source files.

# Directory containing this bash script.
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

find "$DIR/AoE2_Calculator-master" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -exec clang-format -i {} +

exit 0
