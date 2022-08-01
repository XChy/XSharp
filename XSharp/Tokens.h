#pragma once

#include "XString.h"

namespace XSharp
{
	std::unordered_map<XString, bool> keywords={
		{"for",true},
		{"while",true},
		{"break",true},
		{"continue",true},
		{"if",true},
		{"else",true},
		{"return",true},
		{"switch",true},
		{"default",true},
		{"case",true},
		{"throw",true},
		{"catch",true},
		{"try",true},
		{"class",true},
		{"const",true},
		{"enum",true},
		{"false",true},
		{"true",true},
		{"new",true},
		{"public",true},
		{"protected",true},
		{"private",true},
	};
	bool isKeyword(const XString& keyword);

	std::unordered_map<XString, bool> operators = {
		{"+",true},
		{"-",true},
		{"*",true},
		{"/",true},
		{"^",true},
		{"&",true},
		{"%",true},
		{"!",true},
		{"=",true},
		{"==",true},
		{"++",true},
		{"--",true},
		{"<",true},
		{">",true},
		{"<=",true},
		{">=",true},
		{"?",true},
		{":",true},
	};
	bool isOperator(const XString& oper);
	bool isOperator(XChar oper);
}

enum TokenType
{
	Integer,
	Decimal,
	Boolean,
	String,
	Null,
	Operator,
	Comma,
	Dot,
	OpenParenthesis,
	CloseParenthesis,
	OpenBracket,
	CloseBracket,
	OpenBrace,
	CloseBrace,
	SentenceEnd,
	Identifier,
	Keyword
};

class XSharp_EXPORT Token {
public:
	Token() = default;
	Token(TokenType type, const XString& value);

	XString dump() const;

	TokenType type;
	XString value;
};

