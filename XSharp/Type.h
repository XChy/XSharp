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

std::unordered_map<XString, BasicType> nameToBasicType = {
    {"i32", BasicType::I32},     {"i64", BasicType::I64},
    {"ui32", BasicType::UI32},   {"i64", BasicType::UI64},
    {"float", BasicType::Float}, {"double", BasicType::Double},
    {"char", BasicType::Char},   {"boolean", BasicType::Boolean},
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

TypeNode createBasicType(BasicType type);

// params' memory is managed by TypeSystem
TypeNode createFunctionType(TypeNode* returnValueType,
                            std::vector<TypeNode*> paramsType);

TypeNode createArrayType(TypeNode* elementType, uint dimension);

TypeNode createClassType(const XString& baseName);

TypeNode createClosureType();

TypeNode createTypeFor(const XString& baseName);

}  // namespace XSharp
