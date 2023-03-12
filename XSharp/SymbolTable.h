#pragma once
#include <system_error>
#include <unordered_map>
#include <vector>
#include "XSharp/Types/Type.h"
#include "XSharp/XString.h"
#include "XSharp/Symbol.h"

namespace XSharp {

class SymbolTable
{
    typedef std::unordered_map<XString, Symbol>::const_iterator Iterator;

   public:
    SymbolTable();

    void addSymbol(const Symbol& symbol);

    // find the symbols recusively in the table and its parents
    std::vector<Symbol> findSymbols(const XString& name) const;
    std::vector<Symbol> findFunctions(const XString& name) const;
    Symbol findFunctionFor(const XString& name,
                           const std::vector<TypeNode*> argumentTypes) const;

    Symbol findVariable(const XString& name) const;

    // find symbols in current scope
    bool hasSymbol(const XString& name);

    SymbolTable* parent();

    SymbolTable* createChild();

    ~SymbolTable();

   private:
    std::vector<SymbolTable*> children;
    std::unordered_multimap<XString, Symbol> symbols;
    SymbolTable* _parent;
};

}  // namespace XSharp
