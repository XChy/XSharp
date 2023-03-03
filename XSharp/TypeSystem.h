#pragma once

#include <unordered_map>
#include <vector>
#include "xsharp_global.h"
#include "XString.h"
#include "Type.h"

namespace XSharp {

class TypeContext
{
   public:
    TypeContext();
    ~TypeContext();

    TypeNode* registerType(XSharp::TypeNode* type);

    // return typeid
    // If no type with the name exists ,return 0
    uint typeIDOf(XString name);

    TypeNode* typeOf(int typeId);

    std::unordered_map<XString, uint> typesMap;
    std::vector<TypeNode*> typesList;

   private:
    uint registerNum = 0;
};

static TypeContext GlobalTypeContext;
static std::unordered_map<XString, BasicType> nameToBasicType = {
    {"void", BasicType::Void},       {"i32", BasicType::I32},
    {"i64", BasicType::I64},         {"ui32", BasicType::UI32},
    {"ui64", BasicType::UI64},       {"float", BasicType::Float},
    {"double", BasicType::Double},   {"char", BasicType::Char},
    {"boolean", BasicType::Boolean},
};

static std::unordered_map<BasicType, XString> basicTypeToName = {
    {BasicType::Void, "void"},       {BasicType::I32, "i32"},
    {BasicType::I64, "i64"},         {BasicType::UI32, "ui32"},
    {BasicType::UI64, "ui64"},       {BasicType::Float, "float"},
    {BasicType::Double, "double"},   {BasicType::Char, "char"},
    {BasicType::Boolean, "boolean"},
};

TypeNode* getBasicType(BasicType type);

TypeNode* getReferenceType(TypeNode* innerType);

// params' memory is managed by TypeSystem
TypeNode* getFunctionType(TypeNode* returnValueType,
                          std::vector<TypeNode*> paramsType);

TypeNode* getArrayType(TypeNode* elementType, uint dimension);

TypeNode* getClassType(const XString& baseName);

TypeNode* getClosureType();

TypeNode* getTypeFor(const XString& baseName);

}  // namespace XSharp
