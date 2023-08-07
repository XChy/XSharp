#pragma once

#include <unordered_map>
#include <vector>
#include "XSharp/XString.h"
#include "xsharp_global.h"
#include "Tokens.h"
#include "XSharpUtils.h"

namespace XSharp {

static std::unordered_map<XChar, XChar> transferMap = {
    {'0', '\0'}, {'n', '\n'}, {'t', '\t'}, {'\\', '\\'}};

class XSharp_EXPORT Lexer
{
    typedef XString::const_iterator CharIter;

   public:
    Lexer();
    std::vector<Token> tokenize(const XString& source);

    void setSource(const XString& source);
    bool peekIs(TokenType type);
    Token peekToken();
    bool consumeIs(TokenType type);
    void consume();

    static Token fetchFrom(CharIter& iter, Span& span);
    static XString hex(CharIter& iter, Span& span);       // Hexadecimal
    static XString binary(CharIter& iter, Span& span);    // Binary
    static XString floatPoint(CharIter& iter, Span& span, bool& isFP);  // Floating-point

    // the iterator to current Character
};

// Implements consume/peek
class XSharp_EXPORT TokenReader
{
   public:
    TokenReader(Lexer lexer);

   private:
    Lexer* _lexer;
};

}  // namespace XSharp
