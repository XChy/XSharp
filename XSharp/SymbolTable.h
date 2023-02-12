#pragma once
#include <system_error>
#include <unordered_map>
#include "XSharp/XString.h"
#include "XSharp/Symbol.h"

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

    ~SymbolTable();

   private:
    std::vector<SymbolTable*> children;
    std::unordered_map<XString, Symbol> symbols;
    SymbolTable* parent;
};
