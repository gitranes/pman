#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

if (! hash scan-build); then
    echo "scan-build does not exist. Is clang-tools installed?"
    exit 1
fi

C_COMPILER="clang"
BUILD_DIRECTORY="../build"

scan-build --use-cc="$C_COMPILER" cmake -B "$BUILD_DIRECTORY"
scan-build --use-cc="$C_COMPILER" make -C "$BUILD_DIRECTORY" -j
