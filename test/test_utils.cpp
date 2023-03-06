#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include "XSharp/XString.h"
#include "gtest/gtest.h"

std::vector<const char*> corpus = {
    "\n\t\0a1233141",
    "helloworld",
    "\0\0\0",
    "",
    "123112",
    "Nothing is better than programming",
    "Test Test Test Test Test Test Test Test Test Test Test Test Test Test "
    "Test Test Test Test Test Test Test Test Test Test Test Test Test Test "
    "Test Test Test Test Test Test Test Test Test Test Test Test Test Test ",
    "\0 1\0 1\0 4\0 5\0 1\0 4\0"
    "控制台，终端和tty是密切相关的。原本它们都表示一台可以和计算机交互的设备"};

std::vector<int64_t> numbers = {-3,
                                -2,
                                0,
                                1,
                                2,
                                std::numeric_limits<int64_t>::max(),
                                std::numeric_limits<int64_t>::min()};

std::vector<uint64_t> unsignedNumbers = {0, 1, 2, 10086,
                                         std::numeric_limits<uint64_t>::max()};

TEST(XStringTest, FromCString)
{
    for (auto cstr : corpus) {
        ASSERT_STREQ(XString(cstr).toStdString().data(), cstr);
    }
}

TEST(XStringTest, Append)
{
    char buffer[10000] = {0};
    XString strTested;

    for (auto cstr : corpus) {
        strcat(buffer, cstr);
        strTested.append(cstr);
        ASSERT_STREQ(strTested.toStdString().data(), buffer);
    }
}

TEST(XStringTest, convertFromInt)
{
    char buffer[10000] = {0};

    for (auto num : numbers) {
        XString str = XString::fromInterger(num);
        sprintf(buffer, "%lld", num);
        ASSERT_STREQ(str.toStdString().data(), buffer);
    }

    for (auto num : unsignedNumbers) {
        XString str = XString::fromInterger(num);
        sprintf(buffer, "%llu", num);
        ASSERT_STREQ(str.toStdString().data(), buffer);
    }
}
