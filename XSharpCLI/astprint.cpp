#include "XSharp/Lexer.h"
#include "XSharp/Parser.h"
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        printf(
            "Arguments and options missing, please enter '--help' to get "
            "help\n");
        return 1;
    }

    char* path = argv[1];
    char buffer[10028];

    int fd = open(path, O_RDONLY);
    int size = read(fd, buffer, sizeof(buffer));
    buffer[size] = '\0';

    try {
        XSharp::Lexer lexer;
        XSharp::Parser parser;
        auto ast = parser.parse(lexer.tokenize(buffer));
        fmt::print("{}", ast->dump());
        delete ast;

    } catch (XSharpError e) {
        fmt::print("ERROR:{}", e.errorInfo);
    }
    XSharp::Lexer lexer;
    XSharp::Parser parser;

    return 0;
}
