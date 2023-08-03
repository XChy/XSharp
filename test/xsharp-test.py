#!/usr/bin/env python3

import argparse
import os

parser = argparse.ArgumentParser();
allgroup = parser.add_mutually_exclusive_group()
parser.add_argument_group()

parser.add_argument('-v', '--verbose', help="Print test information in detail", action="store_false")

allgroup.add_argument('-a', '--all', help="Test all testsuites", action="store_true")
allgroup.add_argument('--ast', help="Test generating AST", action="store_false"),
allgroup.add_argument('--llvm', help="Test generating LLVM-IR", action="store_false"),
allgroup.add_argument('--java', help="Test generating Java bytecode (Not supported)", action="store_true")

args = parser.parse_args()

project_path = os.path.split(os.path.split(os.path.realpath(__file__))[0])[0]

print(project_path)

