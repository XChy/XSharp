#include "XClass.h"
using namespace XSharp;
int Field::offset()
{
    if (fieldSymbol->symbolType == SymbolType::MemberMethod) {
        return 0;
    } else {
        return fieldSymbol->type->size();
    }
}
