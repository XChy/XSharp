#pragma once
#include "XSharp/ASTNodes.h"
#include "xsharp_global.h"
#include "XSharp/XSharpUtils.h"
#include "SymbolTable.h"

class XSharp_EXPORT Semantics
{
   public:
    Semantics();
    Semantics(ASTNode* ast);

    void analyze(ASTNode* ast);

   private:
    SymbolTable _symbols;
    XSharpError _error;
};
