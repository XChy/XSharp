#include "CLI11.hpp"
#include "LLVMIR/Target.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeConverter.h"
#include "XSharp/XString.h"
#include "fmt/core.h"
#include <LLVMIR/CodeGenerator.h>
#include <XSharp/Lexer.h>
#include <XSharp/Parser.h>
#include <iostream>

#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "XSharp/XSharpEngine.h"
#include "XSharp/XString.h"

int compile(const char *path);

bool isAOT = true;
bool emitLLVMIR = false;
bool hasDefaultOutputName = false;
XString defaultOutputName;
XString projectPath;
std::vector<XString> inputFiles;

int main(int argc, char *argv[])
{
    CLI::App app("The compiler of XSharp");
    app.add_flag("-L,--emit-llvm-ir", emitLLVMIR, "Emit LLVM-IR for the file");
    app.add_flag("-s", isAOT, "Compile XSharp into executable");
    app.add_option("input", inputFiles);
    app.add_option("-o", defaultOutputName, "Where to put the executable");

    CLI11_PARSE(app, argc, argv);
    projectPath = argv[0];
    projectPath =
        projectPath.subString(0, projectPath.lastSubStringIndex("/") + 1);
    projectPath.append("../");

    int optionChar;
    char *argumentStr;


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
    std::unique_ptr<XSharp::ASTNode> ast(parser.parse(tokens));
    fmt::print("{}", ast->dump());

    XSharp::LLVMCodeGen::CodeGenerator helper;
    TypeAdapter::setLLVMBuilder(&helper.ctx.llvm_builder);
    TypeAdapter::setLLVMContext(&helper.ctx.llvm_ctx);

    if(emitLLVMIR){
        helper.generateTextIR(ast.get(), XString(path).append(".ll"));
        return 0;
    }

    helper.generateIR(ast.get(), XString(path).append(".bc"));

    if (!helper.ctx._errors.empty()) {
        std::cout << "Semantic error:\n";
        for (auto error : helper.ctx._errors)
            std::cout << error.errorInfo.toStdString() << "\n";
        return -1;
    }

    auto object_path = XString(path).append(".o");

    auto error_code = emit_object_code(object_path, helper.ctx.module);
    if (error_code.value() != 0) {
        std::cout << error_code.message();
    }

    if (!hasDefaultOutputName) {
        if (XString(path).lastSubStringIndex("xsharp") ==
            strlen(path) - strlen("xsharp")) {
            defaultOutputName = XString(path).subString(
                0, XString(path).lastSubStringIndex("xsharp") - 1);
        } else {
            defaultOutputName = XString(path).append(".out");
        }
    }

    link_object(object_path, projectPath + "lib/libXSharpRuntime.so",
                defaultOutputName);

    return 0;
}
