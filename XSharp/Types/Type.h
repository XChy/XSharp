#pragma once

#include <unordered_map>
#include <vector>
#include <variant>
#include "XSharp/xsharp_global.h"
#include "XSharp/XString.h"

namespace XSharp {

class TypeNode;

enum class BasicType {
    Void = 0,
    I32,  // signed integer
    I64,
    UI32,  // unsigned integer
    UI64,
    Float,
    Double,
    Char,
    Boolean,
    ENDTYPE,  // Applied to traverse
};

struct ReferenceType {
    TypeNode* innerType;
};

class XClass;
struct ClassType {
    XClass* classDecl;
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

// Closure
struct ClosureType {
    std::vector<TypeNode*> paramTypeIDs;
};

class TypeNode
{
   public:
    TypeNode();
    TypeNode(const TypeNode& other);
    bool equals(const TypeNode& other) const;
    bool equals(const TypeNode* other) const { return equals(*other); };
    bool operator==(const TypeNode& other) const { return equals(other); }

    // Basic type
    BasicType basicType() const;
    bool isInteger() const;
    bool isSigned() const;
    bool isUnsigned() const;

    bool isNumber() const;

    uint size() const;

    // Reference type, as variable
    TypeNode* innerType() const;

    // Function type, TODO complete below
    TypeNode* returnValueType() const;
    std::vector<TypeNode*> parameterTypes() const;

    // Array type, TODO complete below
    uint arrayDimension() const;
    TypeNode* elementType() const;

    // Class type,  TODO complete below
    bool isObjectRef() const;
    XClass* getObjectClass() const;

    // generate a unique name for a type
    XString typeName() const;

    uint typeID;
    XString baseName;
    bool isConst;

    enum Categories {
        Basic,
        Reference,
        Array,
        Function,
        Closure,
        Class
    } category;

    std::variant<BasicType, ReferenceType, ClassType, FunctionType, ArrayType,
                 ClosureType>
        typeSpecifiedInfo;
};

};  // namespace XSharp

template <>
struct fmt::formatter<XSharp::TypeNode> {
    constexpr auto parse(format_parse_context& ctx)
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && *it != '}') throw format_error("invalid format");
        return it;
    }

    template <typename FormatContext>
    auto format(XSharp::TypeNode& type, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{}", type.typeName().toStdString());
    }
};
