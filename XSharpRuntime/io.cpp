#include "io.h"
#include <cstdio>
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
