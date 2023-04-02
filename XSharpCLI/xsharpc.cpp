#include <cerrno>
#include <cstdio>
#include <XSharp/Lexer.h>
#include <XSharp/Parser.h>
#include <LLVMIR/LLVMHelper.h>
#include <cstdio>
#include <iostream>
#include "LLVMIR/Target.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeConverter.h"
#include "XSharp/XString.h"
#include "fmt/core.h"

#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "XSharp/XSharpEngine.h"
#include "XSharp/XString.h"

int compile(const char *path);

bool isAOT = true;
bool hasDefaultOutputName = false;
XString defaultOutputName;
std::vector<XString> inputFiles;

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf(
            "Arguments and options missing, please enter '--help' to get "
            "help\n");
        return 1;
    }

    int optionChar;
    char *argumentStr;

    opterr = 0;
    extern char *optarg;

    while ((optionChar = getopt(argc, argv, "-so:")) != -1) {
        switch (optionChar) {
            case 's':
                isAOT = false;
                break;
            case '\1':
                inputFiles.push_back(optarg);
                break;
            case 'o':
                hasDefaultOutputName = true;
                printf(optarg);
                defaultOutputName = optarg;
                break;
            default:
                return -1;
        }
    }

    if (isAOT) {
        return compile(inputFiles[0].toStdString().c_str());
    }

    return 0;
}

int compile(const char *path)
{
    using XSharp::Lexer;
    using XSharp::Parser;
    using XSharp::TypeAdapter;

    char code[2048];

    int fd = open(path, O_RDONLY);

    if (fd == -1) {
        fmt::print("Cannot open file {}", path);
        return -1;
    }

    int size = read(fd, code, 2048);
    code[size] = '\0';
    close(fd);

    printf("Compliation Result of %s:\n", path);

    Lexer lexer;
    auto tokens = lexer.tokenize(code);

    Parser parser;
    auto ast = parser.parse(tokens);
    fmt::print("{}", ast->dump());

    LLVMHelper helper;
    TypeAdapter::setLLVMBuilder(&helper.contextHelper.builder);
    TypeAdapter::setLLVMContext(&helper.contextHelper.context);
    helper.generateLLVMIR(ast, XString(path).append(".bc"));

    if (!helper.contextHelper._errors.empty()) {
        std::cout << "Semantic error:\n";
        for (auto error : helper.contextHelper._errors)
            std::cout << error.errorInfo.toStdString() << "\n";
    }

    auto object_path = XString(path).append(".o").toStdString();

    emit_object_code(object_path, helper.contextHelper.module);

    if (!hasDefaultOutputName) {
        defaultOutputName = XString(path).append(".out");
    }

    link_object(object_path, "./lib/libXSharpRuntime.so",
                defaultOutputName.toStdString());

    delete ast;

    std::cout << std::endl;

    return 0;
}
