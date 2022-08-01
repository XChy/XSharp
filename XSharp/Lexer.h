#pragma once

#include "xsharp_global.h"
#include "Tokens.h"

namespace XSharp {

}

class XSharp_EXPORT Lexer {
public:
	Lexer();
	std::vector<Token> tokenize(const XString& source) const;

};