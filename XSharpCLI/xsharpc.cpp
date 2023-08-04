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

#include "XSharp/XString.h"

int compile(const char *path);

bool isAOT = true;
bool emitLLVMIR = false;
XString OutputFilename;
XString projectPath;
std::vector<XString> inputFile;

int main(int argc, char *argv[])
{
    CLI::App app("The compiler of XSharp");
    app.add_flag("-L,--emit-llvm-ir", emitLLVMIR, "Emit LLVM-IR for the file");
    app.add_flag("-s", isAOT, "Compile XSharp into executable");
    app.add_option("-o", OutputFilename, "Where to put the executable");
    app.add_option("Input files", inputFile, "Input files to compile")
        ->required();

    CLI11_PARSE(app, argc, argv);
    projectPath = argv[0];
    projectPath =
        projectPath.subString(0, projectPath.lastSubStringIndex("/") + 1);
    projectPath.append("../");

    if(OutputFilename.size() == 0){
        auto path = inputFile[0].toStdString().c_str();
        if (XString(path).lastSubStringIndex("xsharp") ==
            strlen(path) - strlen("xsharp"))
            OutputFilename = XString(path).subString(
                0, XString(path).lastSubStringIndex("xsharp") - 1);
        else
            OutputFilename = XString(path).append(".out");
    }

    if (isAOT) return compile(inputFile[0].toStdString().c_str());

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
        fmt::print("Cannot open file {}\n", path);
        return -1;
    }

    int size = read(fd, code, 2048);
    code[size] = '\0';
    close(fd);

    Lexer lexer;
    auto tokens = lexer.tokenize(code);

    Parser parser;
    std::unique_ptr<XSharp::ASTNode> ast(parser.parse(tokens));

    XSharp::LLVMCodeGen::CodeGenerator helper;
    TypeAdapter::setLLVMBuilder(&helper.ctx.llvm_builder);
    TypeAdapter::setLLVMContext(&helper.ctx.llvm_ctx);

    if (emitLLVMIR) {
        helper.generateTextIR(ast.get(), OutputFilename);
        return 0;
    }

    helper.generateIR(ast.get(), OutputFilename.append(".bc"));

    if (!helper.ctx._errors.empty()) {
        fmt::print("Semantic error in {}:\n", path);
        for (auto error : helper.ctx._errors)
            std::cout << error.errorInfo.toStdString() << "\n";
        return -1;
    }

    auto object_path = XString(path).append(".o");

    auto error_code = emit_object_code(object_path, helper.ctx.module);
    if (error_code.value() != 0) std::cout << error_code.message();

    if (OutputFilename.size() == 0) {
        if (XString(path).lastSubStringIndex("xsharp") ==
            strlen(path) - strlen("xsharp")) {
            OutputFilename = XString(path).subString(
                0, XString(path).lastSubStringIndex("xsharp") - 1);
        } else {
            OutputFilename = XString(path).append(".out");
        }
    }

    link_object(object_path, projectPath + "lib/libXSharpRuntime.so",
                OutputFilename);

    return 0;
}
