#include <XSharp/Lexer.h>
#include <XSharp/Parser.h>
#include <LLVMIR/LLVMHelper.h>
#include <cstdio>
#include <iostream>

#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

void test(const char *path);

int main()
{
    try {
        char cases_path[1024] = "./testcases/";
        DIR *working_dir = opendir(cases_path);
        struct dirent *p;
        while ((p = readdir(working_dir)) != NULL) {
            char path[PATH_MAX] = {0};
            strcat(path, cases_path);
            if (p->d_type == 8) {  //  files
                strcat(path, p->d_name);
                test(path);
            } else if (p->d_type == 4) {  // directories
            }
        }
        closedir(working_dir);

    } catch (XSharpError e) {
        std::cout << e.errorInfo.toStdString();
    }
    return 0;
}

void test(const char *path)
{
    char code[2048];

    int fd = open(path, O_RDONLY);
    read(fd, code, 2048);
    close(fd);

    printf("Compliation Result of %s:\n", path);
    Lexer lexer;
    auto tokens = lexer.tokenize(code);

    Parser parser;
    auto ast = parser.parse(tokens);
    std::cout << ast->dump().toStdString();

    LLVMHelper helper;
    helper.generateLLVMIR(ast, "a.bc");

    delete ast;

    std::cout << std::endl;
}
