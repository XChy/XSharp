#include <XSharp/Lexer.h>
#include <XSharp/Parser.h>
#include <LLVMIR/CodeGenerator.h>
#include <cstdio>
#include <iostream>
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeConverter.h"
#include "XSharp/XString.h"
#include "fmt/core.h"

#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void test(const char *path);

int main()
{
    try {
        char cases_path[1024] = "./testcases/";
        DIR *working_dir = opendir(cases_path);
        struct dirent *p;
        while ((p = readdir(working_dir)) != NULL) {
            char path[PATH_MAX] = {0};
            strcpy(path, cases_path);
            if (p->d_type == 8) {  //  files
                strcat(path, p->d_name);
                test(path);
            } else if (p->d_type == 4) {  // directories
            }
        }
        closedir(working_dir);

    } catch (XSharpError e) {
        fmt::print("{}", e.errorInfo);
    }
    return 0;
}

void test(const char *path)
{
    using XSharp::Lexer;
    using XSharp::Parser;
    using XSharp::TypeAdapter;

    if (XString(path).subStringIndex("xsharp") !=
        strlen(path) - strlen("xsharp")) {
        return;
    }

    char code[2048];

    int fd = open(path, O_RDONLY);
    int size = read(fd, code, 2048);
    code[size] = '\0';
    close(fd);

    printf("Compliation Result of %s:\n", path);
    Lexer lexer;
    auto tokens = lexer.tokenize(code);

    Parser parser;
    auto ast = parser.parse(tokens);
    fmt::print("{}", ast->dump());

    XSharp::LLVMCodeGen::CodeGenerator helper;
    TypeAdapter::setLLVMBuilder(&helper.contextHelper.builder);
    TypeAdapter::setLLVMContext(&helper.contextHelper.context);
    helper.generateIR(ast, XString(path).append(".bc"));

    if (!helper.contextHelper._errors.empty()) {
        std::cout << "Semantic error:\n";
        for (auto error : helper.contextHelper._errors)
            std::cout << error.errorInfo.toStdString() << "\n";
    }

    delete ast;

    std::cout << std::endl;
}
