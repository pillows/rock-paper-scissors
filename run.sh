#!/bin/bash

# rm -rf build
mkdir -p ./build/res
cmake -G Ninja -S . -B build -DPLATFORM=Web --toolchain /Users/snow/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
cp -r res/ build/res/
ninja -C build
cp dist/index.html build/

