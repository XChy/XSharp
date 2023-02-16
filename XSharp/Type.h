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
    std::vector<TypeNode*> genericsParams;
};

struct ArrayType {
    uint arrayDimension;
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

// TypeNode has no destructor, for which TypeContext have to manage its memory
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

TypeNode* createBasicType(BasicType type);

TypeNode* createFunctionType(TypeNode* returnValueType,
                             std::vector<TypeNode*> paramsType);

TypeNode* createArrayType(TypeNode* elementType, uint dimension);

TypeNode* createClassType();

TypeNode* createClosureType();

}  // namespace XSharp
