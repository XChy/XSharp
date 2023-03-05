**Not completed yet**

# XSharp

A toy OOP language with lexer, parser, compiler and runtime

### Background

Having learned about the theory of compiler, a dull coder try to develop something to have fun.

XSharp is compiled into the byte codes,and then run in a XSharp runtime environment.

Also it provide a way to generate native binary with [LLVM](https://github.com/llvm/llvm-project)

### Install

This project can be built with CMake

Input the command below in to build XSharp Compiler in your computer(**Only support Linux now**)

```bash
git clone git@github.com:XChy/XSharp.git
cmake . && make #you can configure CMake yourself
```

### Usage

```bash
#default compile into a binary executable
xsharpc xxx.xsharp

#compile into bytecode
xsharpc -vm xxx.xsharp # compile into bytecode which can be executed by XSharp's VM
xsharp xxx.xe          # execute the bytecode
```

### Third-Party

- ##### **LLVM** (**optional**)

  XSharp can compile XSharp code into LLVM IR, which XSharp apply **LLVM14(or above)** to compile and optimize to generate binary

- ##### **FMT**
  For format printing

### Grammar

- #### Variable Declaration

```C++
i32 a = 2333;
```

- #### Function Declaration

```C++
i32 abs(i32 a)
{
   if(a >= 0)
       return a;
	else
       return -a;
}
```

- ### Class Declaration

```C++
class foo
{
    string name();
    int age();

    string _name;
    int _age;
}
// Or in form below
class foo
{
    string _name{get{}, set{}};
    int _age{get{}, set{}};
}
```

- ### Hierarchy of Source

```
--XSharp
 |
 |- XSharp      # Main code of XSharp (Lexer,Parser,Semantics)
 |-
 |- LLVMIR      # Impliment AOT for X# AST with LLVM-IR
 |- XSharpIR    # Impliment VM for X# AST with XSharp-IR
 |- test        # Code to test correctness of XSharp's Builder
 |- testcases   # X# code for test
 |- XSharpCLI   # Command-line Interface(xsharpc, xsharp)
 |- XSharpRuntime # Runtime for X#
```
