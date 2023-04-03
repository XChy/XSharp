#pragma once

#include "XSharp/Lexer.h"
#include "xsharp_global.h"
#include "XSharp/Parser.h"

#ifdef XSharp_LLVMIR_SUPPORT
#include "LLVMIR/CodeGenerator.h"
#endif

namespace XSharp {
class XSharp_EXPORT XSharpEngine
{
   public:
    XSharpEngine();

   private:
    Parser parser;
};
}  // namespace XSharp
