#pragma once

#include <unordered_map>
#include <vector>
#include <variant>
#include "xsharp_global.h"
#include "XString.h"

namespace XSharp {

class TypeNode;

enum class BasicType {
    I32,  // signed integer
    I64,
    UI32,  // unsigned integer
    UI64,
    Float,
    Double,
    Char,
    Boolean,
};

struct ClassType {
    // TODO: complete class type-specified setting
    TypeNode* parentType;
    bool isAbstract;
    // type and name of member and methods
    std::vector<TypeNode*> members;
    std::vector<TypeNode*> memberMethods;
};

struct ArrayType {
    uint arrayDimension;
    uint arraySize;
    TypeNode* elementType;
};

// Function
struct FunctionType {
    std::vector<TypeNode*> paramTypes;
    TypeNode* returnValueType;
};

// Function
struct ClosureType {
    std::vector<uint> paramTypeIDs;
    uint returnValueTypeID;
};

class TypeNode
{
   public:
    TypeNode();
    TypeNode(const TypeNode& other);
    ~TypeNode();
    bool equals(const TypeNode& other) const;

    // Basic type
    BasicType basicType() const;

    // Function type, TODO complete below
    TypeNode* returnValueType() const;
    std::vector<TypeNode*> paramsType() const;

    // Array type, TODO complete below
    uint arrayDimension() const;
    uint arraySize() const;
    TypeNode* elementType() const;

    // Class type,  TODO complete below

    // generate a unique name for a type
    XString typeName() const;

    uint typeID;
    XString baseName;
    bool isConst;
    enum Categories { Basic, Array, Function, Closure, Class } category;

    std::variant<BasicType, ClassType, FunctionType, ArrayType, ClosureType>
        typeSpecifiedInfo;
};

}  // namespace XSharp
