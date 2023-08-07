#! /bin/bash
build_type="${1:-Debug}"

rm -rf ./build
cmake -B ./build -G "Ninja" -DCMAKE_BUILD_TYPE=$build_type
cmake --build ./build --config $build_type