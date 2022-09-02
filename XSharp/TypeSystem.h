#pragma once

#include "xsharp_global.h"
#include "XString.h"

namespace XSharp {
	class BasicType {
	public:
		XString name;
		std::unordered_map<XString, void*> functions;
	};

	struct TypeInfo {
		XString typeName;
		bool isConst;
		int arrayDimension;
	};

	static int registerType(XSharp::BasicType type);//return typeid

	static int typeidOf(XString name);//If no type with the name exists,return -1

	static BasicType* typeOf(int typeId);

	static int registerNum = 0;
	static std::vector<XSharp::BasicType> types;//The index is typeid
}