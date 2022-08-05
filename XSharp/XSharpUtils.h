#pragma once

#include "xsharp_global.h"
#include "XString.h"

class XSharpError {
public:
	XSharpError(const XString& info);
	XString errorInfo;
};