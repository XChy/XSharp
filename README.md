# XSharp
A toy OOP language with lexer, parser, compiler and runtime 

###  Background
Having learned something about the theory of compiler,a dull coder try to develop something to have fun.
XSharp is compiled into the byte codes,and then run in a XSharp runtime environment.

### Install
Just use cmake to generate a library and execuation.

### Usage
Type "XSharpc [your filename]" to generate a "xar" package.
Type "XSharp [your filename]" to run the "xar" package.

###Grammar
- #### Variable Declaration
 ##### [type name] [variable name] = value. For example:
     ```
 i32 a=2333;
```

- #### Function Declaration
 ##### [return type] [type name] [function name] ([type name] param1,[type name] param2,...){[block]}. For example:
     ```
 i32 abs(i32 a){
 	if(a>=0) return a;
	else return -a;
 }
```
Waiting to complete
