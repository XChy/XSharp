#include "io.h"
#include <clocale>
#include <cstdio>
#include <cwchar>

int UCS2toUTF8Code(char16_t ucs2_code, char* utf8_code)
{
    int length = 0;
    char* out = utf8_code;
    if (!utf8_code) {
        return length;
    }
    if (0x0080 > ucs2_code) {
        /* 1 byte UTF-8 Character.*/
        *out = (char)ucs2_code;
        length++;
    } else if (0x0800 > ucs2_code) {
        /*2 bytes UTF-8 Character.*/
        *out = ((char)(ucs2_code >> 6)) | 0xc0;
        *(out + 1) = ((char)(ucs2_code & 0x003F)) | 0x80;
        length += 2;
    } else {
        /* 3 bytes UTF-8 Character .*/
        *out = ((char)(ucs2_code >> 12)) | 0xE0;
        *(out + 1) = ((char)((ucs2_code & 0x0FC0) >> 6)) | 0x80;
        *(out + 2) = ((char)(ucs2_code & 0x003F)) | 0x80;
        length += 3;
    }
    return length;
}

bool printChar(char16_t character) { return printf("%lc", character); }
bool printStr(char16_t* str)
{
    char buf[4];

    while (*str) {
        int len = UCS2toUTF8Code(*str, buf);
        for (int i = 0; i < len; ++i) {
            putchar(buf[i]);
        }
        str++;
    }
}
bool printI32(__int32_t x) { return printf("%d\n", x); }
bool printI64(__int64_t x) { return printf("%ld\n", x); }

bool printFloat(float x) { return printf("%f\n", x); }
bool printDouble(double x) { return printf("%lf\n", x); }

bool printBoolean(bool x) { return printf("%s\n", x ? "true" : "false"); }

__int32_t inputI32()
{
    __int32_t i;
    scanf("%d", &i);
    return i;
}
__int64_t inputI64()
{
    __int64_t i;
    scanf("%ld", &i);
    return i;
}

float inputFloat()
{
    float i;
    scanf("%f", &i);
    return i;
}

double inputDouble()
{
    double i;
    scanf("%lf", &i);
    return i;
}
