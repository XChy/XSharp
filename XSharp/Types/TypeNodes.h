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
    virtual Type* toType() const = 0;
    virtual ~TypeNode() = default;

    virtual XString dump() const;

    Decoration decoration;
};

class IdentifierNode : public TypeNode
{
   public:
    IdentifierNode() {}
    IdentifierNode(const XString& name) : name(name) {}

    Type* toType() const;
    XString dump() const;

    XString name;
};

class ArrayNode : public TypeNode
{
   public:
    ArrayNode();
    ArrayNode(TypeNode* element, int dimension);
    ~ArrayNode();

    Type* toType() const;
    XString dump() const;

    TypeNode* element;
    int dimension;
};

class GenericsNode : public TypeNode
{
   public:
    GenericsNode();
    ~GenericsNode();

    Type* toType() const;
    XString dump() const;

    std::vector<ASTNode*> args;
    TypeNode* prototype;
};

}  // namespace XSharp
