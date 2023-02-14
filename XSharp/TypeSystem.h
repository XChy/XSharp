#pragma once

#include <unordered_map>
#include <vector>
#include "xsharp_global.h"
#include "XString.h"

namespace XSharp {

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

class TypeNode
{
   public:
    TypeNode()
    {  // TODO complete constructor
    }
    TypeNode(const TypeNode& other)
    {
        // TODO complete constructor
    }
    ~TypeNode();
    enum Categories { Class, Function, Array, Closure, Basic } category;

    uint typeID;

    XString baseName;

    bool isConst;

    XString typeName() const;

    union {
        // Basic
        BasicType basicType;
        // Class
        struct {
            uint parentTypeID;
            bool isAbstract;
        };

        // Array
        struct {
            uint arrayDimension;
            uint arraySize;
            uint elementTypeID;
        };

        // Function
        struct {
            std::vector<uint> paramTypeIDs;
            uint returnValueTypeID;
        };
    };
};

class TypeContext
{
   public:
    TypeContext();

    uint registerType(const XSharp::TypeNode& type);

    // return typeid
    // If no type with the name exists ,return 0
    uint typeIDOf(XString name);

    TypeNode* typeOf(int typeId);

    std::unordered_map<XString, uint> typesMap;
    std::vector<TypeNode*> typesList;

    ~TypeContext();

   private:
    uint registerNum = 0;
};

}  // namespace XSharp
