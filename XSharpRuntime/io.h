#pragma once
#include <stdio.h>
#include <stdbool.h>

// print scan related function
extern "C" bool printI32(__int32_t x);
extern "C" bool printI64(__int64_t x);
extern "C" bool printFloat(float x);
extern "C" bool printDouble(double x);
extern "C" bool printBoolean(bool x);

extern "C" __int32_t inputI32();
extern "C" __int64_t inputI64();
extern "C" float inputFloat();
extern "C" double inputDouble();
