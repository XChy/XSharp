#include "XSharp/XString.h"
#include "gtest/gtest.h"

TEST(XStringTest, FromCString)
{
    std::vector<const char*> v = {
        "\n\t\0a1233141",
        "helloworld",
        "\0\0\0",
        "",
        "123112",
        "Nothing is better than programming",
        "Test Test Test Test Test Test Test Test Test Test Test Test Test Test "
        "Test Test Test Test"};
    for (auto cstr : v) {
        ASSERT_STREQ(XString(cstr).toStdString().data(), cstr);
    }
}

TEST(XStringTest, Append)
{
    std::vector<const char*> v = {"\n\t\0a1233141", "helloworld", "\0", "",
                                  "123112"};
    for (auto cstr : v) {
        ASSERT_STREQ(XString(cstr).toStdString().data(), cstr);
    }
}

// int main()
//{
// XString s = "12345";
// for (int i = 0; i < 1000; ++i) {
// char buffer[100];
// sprintf(buffer, ",%d", i);
// s.append("[" + XString(buffer) + "]");
//}
// std::cout << s.toStdString() << std::endl;
// return 0;
//}
