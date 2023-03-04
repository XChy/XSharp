#include "XSharp/SymbolTable.h"

using namespace XSharp;

SymbolTable::SymbolTable() : _parent(nullptr) {}

void SymbolTable::addSymbol(const Symbol& symbol)
{
    symbols[symbol.name] = symbol;
}

SymbolTable::Iterator SymbolTable::findSymbol(const XString& name)
{
    if (children.empty()) {
        return symbols.find(name);
    }

    Iterator currentIt = symbols.find(name);
    if (currentIt == this->end()) {
        Iterator parentIt = _parent->findSymbol(name);

        if (parentIt == _parent->end())
            return this->end();
        else
            return parentIt;
    } else {
        return currentIt;
    }
}

SymbolTable::Iterator SymbolTable::begin() { return symbols.begin(); }
SymbolTable::Iterator SymbolTable::end() { return symbols.end(); }

bool SymbolTable::hasSymbol(const XString& name)
{
    return symbols.find(name) != symbols.end();
}

Symbol& SymbolTable::operator[](const XString& name)
{
    return findSymbol(name)->second;
}

Symbol SymbolTable::at(const XString& name) { return findSymbol(name)->second; }

SymbolTable* SymbolTable::parent() { return _parent; }

SymbolTable* SymbolTable::createChild()
{
    SymbolTable* child = new SymbolTable;
    child->_parent = this;
    children.push_back(child);
    return child;
}

SymbolTable::~SymbolTable()
{
    for (SymbolTable* child : children) {
        if (child) delete child;
    }
}
