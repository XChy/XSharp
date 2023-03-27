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

    virtual XString dump() const;
};

class IdentifierNode : public TypeNode
{
   public:
    IdentifierNode() {}

    Type* toType();
    XString dump() const;

    XString name;
};

class ArrayNode : public TypeNode
{
   public:
    ArrayNode();
    ~ArrayNode();

    virtual Type* toType();
    XString dump() const;

    TypeNode* elementType;
    int dimensions;
};

class GenericsNode : public TypeNode
{
   public:
    GenericsNode();
    ~GenericsNode();

    XString dump() const;

    std::vector<ASTNode*> args;
};

}  // namespace XSharp
