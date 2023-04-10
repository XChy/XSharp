#pragma once

#include <vector>
#include "XSharp/XString.h"
#include "xsharp_global.h"
#include "Tokens.h"
#include "XSharpUtils.h"

namespace XSharp {

class XSharp_EXPORT Lexer
{
   public:
    Lexer();
    std::vector<Token> tokenize(const XString& source);

   private:
    XString hex();  // Hexadecimal
    XString bin();  // Binary
    Token dec();    // Decimal

    bool next();
    bool isEof();
    XChar peek() const;
    XChar cur() const;

    Span currentSpan() const;

    // the iterator to current Character
    XString::const_iterator currentIter;

    // Span information
    XString filename;
    uint row;
    uint col;
};
}  // namespace XSharp
