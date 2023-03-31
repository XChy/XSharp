#pragma once
#include "XSharp/ASTNodes.h"
#include "XSharp/Parser.h"
#include "XSharp/xsharp_global.h"
#include "XSharp/SymbolTable.h"
#include "XSharp/Types/TypeSystem.h"

namespace XSharp {

class XSharpContext
{
    friend class Parser;

   public:
    XSharpContext();
    ~XSharpContext();

   private:
    SymbolTable _globalSymbols;

    TypeContext _typeContext;

    ASTNode* _ast;
};

}  // namespace XSharp
