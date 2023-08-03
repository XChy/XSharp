#!/usr/bin/env python3

import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument(
    "-v", "--verbose", help="Print test information in detail", action="store_false"
)

args = parser.parse_args()

project_path = os.path.split(os.path.split(os.path.realpath(__file__))[0])[0]

bin_path = os.path.join(project_path, "bin")
xsharpc_path = os.path.join(bin_path, "xsharpc")

# update LLVM-IR
path = os.path.join(project_path, "LLVMIR/test")
files = os.listdir(path)
for file in files:
    file_path = os.path.join(path, file)

    if file.endswith(".xsharp"):
        os.popen(xsharpc_path + " --emit-llvm-ir " + file_path)

# update AST
# path = os.path.join(project_path, "LLVMIR/test")
# files = os.listdir(path)
# for file in files:
# file_path = os.path.join(path, file)

# if file.endswith(".xsharp"):
# os.popen(xsharpc_path + " --emit-llvm-ir " + file_path)
