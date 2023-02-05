#include "XSharpUtils.h"

XSharpError::XSharpError() : errorType(NoError) {}
XSharpError::XSharpError(const XString& info)
    : errorInfo(info), errorType(NoError)
{
}

XSharpError::XSharpError(XSharpErrorType type, const XString& info)
    : errorType(type), errorInfo(info)
{
}
