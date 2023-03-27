#pragma once

#include "XSharp/ASTNodes.h"
#include "XSharp/Tokens.h"
#include "XSharp/Types/Type.h"
#include "XSharp/XString.h"
#include "XSharp/xsharp_global.h"

namespace XSharp {

class TypeNode : public ASTNode
{
   public:
    virtual Type* toType() = 0;
    virtual ~TypeNode() = default;
};

class IdentifierNode : public TypeNode
{
   public:
    IdentifierNode() {}
    virtual Type* toType();

    XString name;
};

class ArrayNode : public TypeNode
{
   public:
    ArrayNode();
    ~ArrayNode();
    virtual Type* toType();

    TypeNode* elementType;
    int dimensions;
};

class GenericsNode : public TypeNode
{
   public:
    GenericsNode();
    ~GenericsNode();

    std::vector<ASTNode*> args;
};

}  // namespace XSharp
