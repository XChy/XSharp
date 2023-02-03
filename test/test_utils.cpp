#include "XSharp/XString.h"
#include <cstdio>
#include <iostream>

int main()
{
    XString s = "12345";
    for (int i = 0; i < 1000; ++i) {
        char buffer[100];
        sprintf(buffer, ",%d", i);
        s.append("[" + XString(buffer) + "]");
    }
    std::cout << s.toStdString() << std::endl;
    return 0;
}
