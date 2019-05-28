#!/bin/bash

set -e

[ -z "${SRC_DIR}" ] && export SRC_DIR="`pwd`"
[ -z "${BUILD_DIR}" ] && export BUILD_DIR=/build
mkdir -p "${BUILD_DIR}"

BAZEL_OPTIONS="--jobs 1"
BAZEL_TEST_OPTIONS="$BAZEL_OPTIONS --test_output=errors"


if [[ "$1" == "clang_tidy" ]]; then
  export CC=/usr/bin/clang-6.0
  CC=/usr/bin/clang-6.0 bazel build \
        $BAZEL_OPTIONS \
        //...
  ./ci/gen_compilation_database.sh
  ./ci/fix_compilation_database.py
  ./ci/run_clang_tidy.sh |& tee /clang-tidy-result.txt
  grep ": error:" /clang-tidy-result.txt | cat > /clang-tidy-errors.txt
  num_errors=`wc -l /clang-tidy-errors.txt | awk '{print $1}'`
  if [[ $num_errors -ne 0 ]]; then
    exit 1
  fi
  exit 0
elif [[ "$1" == "test" ]]; then
  bazel test $BAZEL_TEST_OPTIONS -c dbg //...
  exit 0
fi
