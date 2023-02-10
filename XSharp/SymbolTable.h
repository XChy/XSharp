#pragma once
#include <system_error>
#include "xsharp_global.h"

class SymbolTable
{
   public:
    SymbolTable();

   private:
    std::vector<SymbolTable*> children;
    SymbolTable* parent;
};
