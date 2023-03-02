#!/usr/bin/env bash
projectDir=$(dirname $0)

cmake . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make

# Install X#'s' Runtime
sudo cp $projectDir/lib/libXSharpRuntime.so /usr/lib
