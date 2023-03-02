#!/usr/bin/env bash

name=$0

projectDir=$(dirname $0)

targetName=$1

${projectDir}/test/bin/XSharp_TEST

llc -filetype=obj ${projectDir}/testcases/${targetName}.xsharp.bc

gcc ${projectDir}/testcases/${targetName}.xsharp.o ${projectDir}/lib/libXSharpRuntime.so -o ${projectDir}/testcases/${targetName}
