#pragma once

#include <unordered_map>
#include <vector>
#include <variant>
#include "XSharp/xsharp_global.h"
#include "XSharp/XString.h"
#include "XSharp/Class/XClass.h"

namespace XSharp {

class Type;
class XClass;

enum class BasicType {
    Void = 0,
    I8,
    I16,
    I32,  // signed integer
    I64,
    UI8,
    UI16,
    UI32,
    UI64,
    Float,
    Double,
    Char,
    Boolean,
    ENDTYPE,  // Applied to traverse
};

struct ReferenceType {
    Type* derefType;
};

struct ClassType {
    XClass* classDecl;
};

struct ArrayType {
    uint arrayDimension;
    Type* elementType;
};

// Function
struct FunctionType {
    std::vector<Type*> paramTypes;
    Type* returnValueType;
};

// Closure
struct ClosureType {
    std::vector<Type*> paramTypeIDs;
};

struct Decoration {
    bool isMutable;
    bool isConstexpr;
};

class Type
{
   public:
    Type();
    Type(const Type& other);
    bool equals(const Type& other) const;
    bool equals(const Type* other) const { return equals(*other); };
    bool operator==(const Type& other) const { return equals(other); }

    // Basic type
    bool isBasic() const;
    BasicType basicType() const;
    bool isInteger() const;
    bool isSigned() const;
    bool isUnsigned() const;

    bool isNumber() const;

    uint size() const;
    uint bits() const;

    // Reference type, as variable
    bool isRef() const;
    Type* derefType() const;

    // Function type, TODO complete below
    Type* returnValueType() const;
    std::vector<Type*> parameterTypes() const;

    // Array type, TODO complete below
    uint arrayDimension() const;
    Type* elementType() const;

    // Class type,  TODO complete below
    bool isObjectRef() const;
    XClass* getObjectClass() const;

    // generate a unique name for a type
    XString typeName() const;

    uint typeID;
    XString baseName;
    bool isConst;

    Decoration decoration;

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
        typeinfo;
};

};  // namespace XSharp

template <>
struct fmt::formatter<XSharp::Type> {
    constexpr auto parse(format_parse_context& ctx)
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && *it != '}') throw format_error("invalid format");
        return it;
    }

    template <typename FormatContext>
    auto format(XSharp::Type& type, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{}", type.typeName().toStdString());
    }
};
