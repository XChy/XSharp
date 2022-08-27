#pragma once

#include "xsharp_global.h"
#include "Assoc.h"
#include "XString.h"

namespace XSharp {
	static std::unordered_map<XString, int> binaryOperatorPriority = {
		{"+",3},
		{"-",3},
		{"*",4},
		{"/",4},
		{"^",11},
		{"&",10},
		{"%",4},
		{"!=",9},
		{"=",16},
		{"==",9},
		{"+=",16},
		{"-=",16},
		{"*=",16},
		{"/=",16},
		{"<<=",16},
		{">>=",16},
		{"%=",16},
		{"&=",16},
		{"|=",16},
		{"^=",16},
		{"<",8},
		{">",8},
		{"<=",8},
		{">=",8},
		{"?",15},
		{":",15},
	};


}

class XSharpError {
public:
	XSharpError(const XString& info);
	XString errorInfo;
};