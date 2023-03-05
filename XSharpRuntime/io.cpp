#include "io.h"
bool printI32(__int32_t x) { return printf("%d", x); }
bool printI64(__int64_t x) { return printf("%ld", x); }

bool printFloat(float x) { return printf("%f", x); }
bool printDouble(double x) { return printf("%lf", x); }

bool printString() {}
