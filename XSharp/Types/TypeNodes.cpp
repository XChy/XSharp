#include "TypeNodes.h"

using namespace XSharp;

ArrayNode::ArrayNode() : elementType(nullptr) {}
ArrayNode::~ArrayNode() { delete elementType; }

GenericsNode::GenericsNode() {}
GenericsNode::~GenericsNode()
{
    for (auto p : args) {
        delete p;
    }
}
