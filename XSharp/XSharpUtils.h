#pragma once

#include "xsharp_global.h"
#include "XString.h"

/*! \enum XSharpErrorType
 *
 *  Classify different types of error
 */
enum XSharpErrorType {
    NoError,
    TokenizingError,
    ParsingError,
    SemanticsError,
    CodeGenError
};

class XSharp_EXPORT XSharpError
{
   public:
    XSharpError();
    XSharpError(const XString& info);
    XSharpError(XSharpErrorType type, const XString& info);
    XString errorInfo;
    XSharpErrorType errorType;
};
