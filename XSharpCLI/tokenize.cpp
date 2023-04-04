#include "XSharp/Lexer.h"
#include "fmt/core.h"
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
        auto tokens = (lexer.tokenize(buffer));
        std::vector<std::string> dumps;
        for (auto token : tokens) {
            dumps.push_back(token.dump().toStdString());
        }

        fmt::print("{}\n", fmt::join(dumps, ", "));
    } catch (XSharpError& e) {
        fmt::print("ERROR:{}\n", e.errorInfo);
    }

    return 0;
}
