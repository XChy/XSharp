#include <cerrno>
#include <cstdio>
#include "XSharp/XSharpEngine.h"
#include "XSharp/XString.h"

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

    while ((optionChar = getopt(argc, argv, "so:")) != -1) {
        switch (optionChar) {
            case 'l':
                // LLVM IR
                break;
            case 'r':
                // XSharp IR
                break;
            case 'o':
                // Output Object File
                break;
            default:
                printf("Invalid Option: %c", optionChar);
                return -1;
        }
    }

    return 0;
}
