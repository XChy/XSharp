#include <XSharp/Lexer.h>
#include <XSharp/Parser.h>
#include <LLVMIR/LLVMHelper.h>
#include <cstdio>
#include <iostream>

int main()
{
    while (true) {
        try {
            Lexer lexer;
            auto tokens = lexer.tokenize("i32 a;i64 abc;float fs;");

            Parser parser;
            std::cout << parser.parse(tokens)->dump().toStdString();

            LLVMHelper helper;
            helper.generateLLVMIR(parser.parse(tokens), "a.bc");

            std::cout << std::endl;

            std::string a;
            std::cin >> a;
        } catch (XSharpError e) {
            std::cout << e.errorInfo.toStdString();
        }
    }
    return 0;
}
