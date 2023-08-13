#! /bin/bash
build_type="${1:-Debug}"

cmake -B ./build/$build_type -G "Ninja" -DCMAKE_BUILD_TYPE=$build_type
cmake --build ./build/$build_type --config $build_type