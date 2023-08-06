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
astprint_path = os.path.join(bin_path, "astprint")
tokenize_path = os.path.join(bin_path, "tokenize")

# update LLVM-IR
llvm_test_path = os.path.join(project_path, "LLVMIR", "test")
files = os.listdir(llvm_test_path)
for file in files:
    file_path = os.path.join(llvm_test_path, file)
    output_path = file_path + ".ll"

    if file.endswith(".xsharp"):
        os.system(xsharpc_path + " " + file_path + " -L " + "-o" + output_path)

# update AST
path = os.path.join(project_path, "XSharp/test/Parser")
files = os.listdir(path)
for file in files:
    file_path = os.path.join(path, file)
    output_path = file_path + "_ast.txt"

    if file.endswith(".xsharp"):
        os.system(astprint_path + " " + file_path + " -o " + output_path)

# update tokens
path = os.path.join(project_path, "XSharp/test/Lexer")
files = os.listdir(path)
for file in files:
    file_path = os.path.join(path, file)
    output_path = file_path + "_tokens.txt"

    if file.endswith(".xsharp"):
        os.system(tokenize_path + " " + file_path + " -o " + output_path)
