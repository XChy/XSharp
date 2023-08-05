#!/usr/bin/env python3

import argparse
import difflib
import os

parser = argparse.ArgumentParser()
allgroup = parser.add_mutually_exclusive_group()
parser.add_argument_group()

parser.add_argument(
    "-v", "--verbose", help="Print test information in detail", action="store_true"
)
allgroup.add_argument("-a", "--all", help="Test all testsuites", action="store_true")
allgroup.add_argument("--ast", help="Test generating AST", action="store_false"),
allgroup.add_argument("--llvm", help="Test generating LLVM-IR", action="store_false"),
allgroup.add_argument(
    "--java", help="Test generating Java bytecode (Not supported)", action="store_true"
)

args = parser.parse_args()

project_path = os.path.split(os.path.split(os.path.realpath(__file__))[0])[0]
bin_path = os.path.join(project_path, "bin")
xsharpc_path = os.path.join(bin_path, "xsharpc")
astprint_path = os.path.join(bin_path, "astprint")

total_test = 0
failure_test = 0

# Test LLVM-IR generation
llvm_test_path = os.path.join(project_path, "LLVMIR/test")
files = os.listdir(llvm_test_path)

for file in files:
    file_path = os.path.join(llvm_test_path, file)

    if file.endswith(".xsharp"):
        source_path = file_path
        ir_path = file_path + ".ll"
        total_test += 1

        with open(source_path, mode="r") as source:
            os.makedirs(os.path.join(llvm_test_path, "temp"), exist_ok=True)
            temp_path = os.path.join(llvm_test_path, "temp", file + ".ll")
            code = os.system(
                xsharpc_path + " --emit-llvm-ir " + file_path + " -o " + temp_path
            )

        with open(temp_path, mode="r") as temp:
            temp_lines = temp.readlines()

        with open(ir_path, mode="r") as ir:
            ir_lines = ir.readlines()

        if ir_lines != temp_lines:
            failure_test += 1
            print("[{}] [LLVM] Test failed at {}:".format(total_test, source_path))

            if args.verbose:
                print("".join(list(difflib.ndiff(ir_lines, temp_lines))))
        else:
            print("[{}] [LLVM] Test passed {}:".format(total_test, source_path))

# Test Parser
parser_test_path = os.path.join(project_path, "XSharp/test/Parser")
files = os.listdir(llvm_test_path)

for file in files:
    file_path = os.path.join(parser_test_path, file)

    if file.endswith(".xsharp"):
        source_path = file_path
        ir_path = file_path + "_ast.txt"
        total_test += 1

        with open(source_path, mode="r") as source:
            os.makedirs(os.path.join(parser_test_path, "temp"), exist_ok=True)
            temp_path = os.path.join(parser_test_path, "temp", file + "_ast.txt")
            code = os.system(astprint_path + " " + file_path + " -o " + temp_path)

        with open(temp_path, mode="r") as temp:
            temp_lines = temp.readlines()

        with open(ir_path, mode="r") as ir:
            ir_lines = ir.readlines()

        if ir_lines != temp_lines:
            failure_test += 1
            print("[{}] [Parser] Test failed at {}:".format(total_test, source_path))

            if args.verbose:
                print("".join(list(difflib.ndiff(ir_lines, temp_lines))))
        else:
            print("[{}] [Parser] Test passed {}:".format(total_test, source_path))


print("===================Test Completed=======================")
print("Total {} tests".format(total_test))
print("Passed {} tests".format(total_test - failure_test))
print("Failed {} tests".format(failure_test))
