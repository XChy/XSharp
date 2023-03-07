#include "XSharp/SymbolTable.h"
#include <algorithm>
#include "XSharp/Symbol.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeSystem.h"

using namespace XSharp;

SymbolTable::SymbolTable() : _parent(nullptr) {}

void SymbolTable::addSymbol(const Symbol& symbol)
{
    symbols.insert({symbol.name, symbol});
}

std::vector<Symbol> SymbolTable::findSymbols(const XString& name) const
{
    Iterator symbolIterator = symbols.find(name);

    if (symbolIterator == symbols.end()) {
        if (_parent)
            return _parent->findSymbols(name);
        else
            return {};
    }

    auto symbolCount = symbols.count(name);
    std::vector<Symbol> result;
    for (int i = 0; i < symbolCount; ++i) {
        result.push_back(symbolIterator->second);
        ++symbolIterator;
    }
    return result;
}

std::vector<Symbol> SymbolTable::findFunctions(const XString& name) const
{
    Iterator symbolIterator = symbols.find(name);
    auto symbolCount = symbols.count(name);
    std::vector<Symbol> result;
    for (int i = 0; i < symbolCount; ++i) {
        if (symbolIterator->second.symbolType == SymbolType::Function)
            result.push_back(symbolIterator->second);

        ++symbolIterator;
    }

    if (result.empty()) {
        if (_parent)
            return _parent->findSymbols(name);
        else
            return {};
    }

    return result;
}

Symbol SymbolTable::findFunctionFor(
    const XString& name, const std::vector<TypeNode*> argumentTypes) const
{
    auto functions = findFunctions(name);
    for (auto funcSymbol : functions) {
        auto parameterTypes = funcSymbol.type->parameterTypes();
        if (std::equal(argumentTypes.begin(), argumentTypes.end(),
                       parameterTypes.begin(), parameterTypes.end(),
                       [](TypeNode* a, TypeNode* b) -> bool {
                           return a->equals(b);
                       })) {
            return funcSymbol;
        }
    }

    // for (auto funcSymbol : functions) {
    // auto parameterTypes = funcSymbol.type->parameterTypes();
    // bool adaptable = std::equal(
    // parameterTypes.begin(), parameterTypes.end(), argumentTypes.begin(),
    // argumentTypes.end(),
    //[](auto parameterType, auto argumentType) -> bool {
    // return TypeAdapter::canConvert(argumentType, parameterType);
    //});
    // if (adaptable) return funcSymbol;
    //}
    return Symbol{.symbolType = SymbolType::NoneSymbol};
}

Symbol SymbolTable::findVariable(const XString& name) const
{
    auto foundSymbols = findSymbols(name);

    for (auto symbol : foundSymbols) {
        if (symbol.symbolType == SymbolType::LocalVariable ||
            symbol.symbolType == SymbolType::GlobalVariable ||
            symbol.symbolType == SymbolType::Argument) {
            return symbol;
        }
    }

    return Symbol{.symbolType = SymbolType::NoneSymbol};
}
bool SymbolTable::hasSymbol(const XString& name)
{
    return symbols.find(name) != symbols.end();
}

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
