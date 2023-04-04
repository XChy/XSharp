#pragma once

#include "XSharp/xsharp_global.h"
#include "XSharp/XString.h"
#include "Type.h"

namespace XSharp {

namespace Types {
Type* get(const XString& name);
}  // namespace Types

class TypeContext
{
   public:
    TypeContext();
    ~TypeContext();

    void registerType(Type* type);
    void registerTypeIf(Type* type);

    std::unordered_map<XString, Type*> types;
    std::unordered_map<XString, XClass*> classDecls;
    std::unordered_map<BasicType, XString> basicTypeToName;

   private:
    uint registerNum = 0;
};

static std::unordered_map<XString, BasicType> nameToBasicType = {
    {"void", BasicType::Void},       {"i8", BasicType::I8},
    {"ui8", BasicType::UI8},         {"i16", BasicType::I16},
    {"ui16", BasicType::UI16},       {"i32", BasicType::I32},
    {"i64", BasicType::I64},         {"ui32", BasicType::UI32},
    {"ui64", BasicType::UI64},       {"float", BasicType::Float},
    {"double", BasicType::Double},   {"char", BasicType::Char},
    {"boolean", BasicType::Boolean},
};

TypeContext* getTypeContext();

Type* getBasicType(BasicType type);

Type* getReferenceType(Type* innerType);

// params' memory is managed by TypeSystem
Type* getFunctionType(Type* returnValueType,
                      const std::vector<Type*>& paramsType);

Type* getArrayType(Type* elementType, uint dimension);

Type* getClassType(const XString& baseName);

Type* getClosureType();

// To adapt the lhs number and rhs number for logical and arithmetic operator
Type* getMergedType(Type* lhs_type, Type* rhs_type);

Type* registerClass(XClass* classDecl);

inline Type* getVoidType() { return getBasicType(BasicType::Void); }
inline Type* getI64Type() { return getBasicType(BasicType::I64); }
inline Type* getI32Type() { return getBasicType(BasicType::I32); }
inline Type* getUI32Type() { return getBasicType(BasicType::UI32); }
inline Type* getUI64Type() { return getBasicType(BasicType::UI64); }
inline Type* getFloatType() { return getBasicType(BasicType::Float); }
inline Type* getDoubleType() { return getBasicType(BasicType::Double); }
inline Type* getBooleanType() { return getBasicType(BasicType::Boolean); }
inline Type* getCharType() { return getBasicType(BasicType::Char); }

}  // namespace XSharp
