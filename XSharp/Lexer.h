#pragma once

#include "xsharp_global.h"
#include "Tokens.h"

namespace XSharp {

}

class XSharp_EXPORT Lexer {
public:
	Lexer();
	std::vector<Token> tokenize(const XString& source) const;
private:
	XString hex(XString::const_iterator& it) const;//Hexadecimal
	XString bin(XString::const_iterator& it) const;//Binary
	Token dec(XString::const_iterator& it) const;//Decimal
};