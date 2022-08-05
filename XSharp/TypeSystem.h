#pragma once

#include "xsharp_global.h"
#include "XString.h"

namespace XSharp {
	class Type {
	public:
		XString name;
	};

	static int registerType(XSharp::Type type);//return typeid

	static int typeidOf(XString name);

	static int registerNum=0;
	static std::vector<XSharp::Type> types;//The index is typeid
}