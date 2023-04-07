# XSharp

A toy OOP language with lexer, parser, compiler and runtime

**Not completed yet, and your contributions are highly appreciated**

### Background

Having learned about the theory of compiler, a dull coder try to develop something to have fun.

XSharp is compiled into the byte codes,and then run in a XSharp runtime environment.

Also it provide a way to generate native binary with [LLVM](https://github.com/llvm/llvm-project)

### Install

This project can be built with CMake

Input the command below in to build XSharp Compiler in your computer(**Only support Linux now**)

```bash
# Install LLVM dependencies for XSharp
sudo apt install llvm-dev

# Install toolchain for compiling XSharp
sudo apt install build-essential

git clone git@github.com:XChy/XSharp.git where/xsharp/lies/in

cd where/xsharp/lies/in
./buildRelease.sh
```

### Usage

```bash
#default compile into a binary executable
bin/xsharpc xxx.xsharp

#compile into bytecode, not supported yet
bin/xsharpc -vm xxx.xsharp # compile into bytecode which can be executed by XSharp's VM
bin/xsharp xxx.xe          # execute the bytecode
```

### Third-Party

- ##### **LLVM** (**optional**)

  XSharp can compile XSharp code into LLVM IR, which XSharp apply **LLVM14(or above)** to compile and optimize to generate binary

- ##### **FMT**

  For format printing

- ##### **Boehm GC** (**Temporary GC, I may develop a unique one for XSharp later**)

  For garbage collection

### TODOs

- ##### simple OOP
- ##### basic types
- ##### complete type conversion rules
- ##### module
- ##### match pattern

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
    i32 getAge()
    {
        return self.age;
    }

    void setAge(i32 age)
    {
        self.age = age;
        return; // I will implement automatical return in the future
    }

    i32 age;
}

```

- ### Hierarchy of Source

```
--XSharp
 |
 |- XSharp      # Main code of XSharp (Lexer,Parser,Semantics)
 |- XSharpIR    # Impliment VM for X# AST with XSharp-IR
 |- XSharpRuntime # Runtime for X#
 |- XSharpCLI   # Command-line Interface(xsharpc, xsharp)
 |-
 |- LLVMIR      # Impliment AOT for X# AST with LLVM-IR
 |- test        # Code to test correctness of XSharp's Builder
 |- testcases   # X# code for test
```
