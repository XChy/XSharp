#include "XSharp/Lexer.h"
#include "XSharp/Parser.h"
#include <unistd.h>
#include "CLI11.hpp"
#include "fmt/core.h"
#include <fcntl.h>
#include <fstream>

XString inputFile;
XString OutputFilename;

int main(int argc, char* argv[])
{
    CLI::App app("Print AST of XSharp");
    app.add_option("-o", OutputFilename, "Where to put the executable");
    app.add_option("Input files", inputFile, "Input files to compile")
        ->required();
    CLI11_PARSE(app, argc, argv);

    char* path = argv[1];
    char buffer[10028];

    int fd = open(path, O_RDONLY);
    int size = read(fd, buffer, sizeof(buffer));
    buffer[size] = '\0';

    try {
        XSharp::Lexer lexer;
        XSharp::Parser parser;

        auto ast = parser.parse(lexer.tokenize(buffer));

        if (OutputFilename.size() == 0) {
            fmt::print("{}", ast->dump());
        } else {
            std::ofstream output(OutputFilename.toStdString());
            output << ast->dump().toStdString();
            output.close();
        }

        delete ast;
    } catch (XSharpError& e) {
        fmt::print("ERROR:{}\n", e.errorInfo);
    }

    return 0;
}
