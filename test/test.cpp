#include <XSharp/Lexer.h>
#include <XSharp/Parser.h>
#include <LLVMIR/LLVMHelper.h>
#include <cstdio>
#include <iostream>
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

    using XSharp::Parser;
    Parser parser;
    auto ast = parser.parse(tokens);
    fmt::print("{}", ast->dump());

    LLVMHelper helper;
    helper.generateLLVMIR(ast, XString(path).append(".bc"));

    delete ast;

    std::cout << std::endl;
}
