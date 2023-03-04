#pragma once
#include <system_error>
#include <unordered_map>
#include "XSharp/XString.h"
#include "XSharp/Symbol.h"

namespace XSharp {

class SymbolTable
{
    typedef std::unordered_map<XString, Symbol>::iterator Iterator;

   public:
    SymbolTable();

    void addSymbol(const Symbol& symbol);

    Iterator findSymbol(const XString& name);
    Iterator begin();
    Iterator end();

    bool hasSymbol(const XString& name);

    Symbol& operator[](const XString& name);
    Symbol at(const XString& name);

    SymbolTable* parent();

    SymbolTable* createChild();

    ~SymbolTable();

   private:
    std::vector<SymbolTable*> children;
    std::unordered_map<XString, Symbol> symbols;
    SymbolTable* _parent;
};

}  // namespace XSharp
