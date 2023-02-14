#include "TypeSystem.h"
#include <algorithm>

using namespace XSharp;

TypeNode::~TypeNode()
{
    // TODO complete deconstructor
}

TypeContext::TypeContext()
{
    // fill a useless node so that typeID start with 1
    typesList.push_back(new TypeNode);
}

uint TypeContext::registerType(const XSharp::TypeNode& type)
{
    registerNum++;
    TypeNode* newNode = new TypeNode(type);

    newNode->typeID = registerNum;
    typesMap[type.typeName()] = registerNum;
    return registerNum;
}

XString TypeNode::typeName() const
{
    switch (category) {
        // TODO:complete full type name generation
    }
}

TypeNode* TypeContext::typeOf(int typeId) { return typesList[typeId]; }

uint TypeContext::typeIDOf(XString name)
{
    auto iterator_found = typesMap.find(name);

    if (iterator_found != typesMap.end()) {
        return iterator_found->second;
    }

    return 0;
}

TypeContext::~TypeContext()
{
    for (TypeNode* type : typesList) delete type;
}
