#include "TypeSystem.h"

using namespace XSharp;

int XSharp::registerType(XSharp::BasicType)
{
	registerNum++;
	return registerNum - 1;
}

int XSharp::typeidOf(XString name)
{
	int id = -1;
	for (int i = 0; i < types.size(); i++) {
		if (types[i].name == name) {
			id = i;
		}
	}
	return id;
}

BasicType* XSharp::typeOf(int typeId)
{
	return &types[typeId];
}
