#pragma once

#include "xsharp_global.h"
#include "XString.h"
// Assocativity
enum Assoc { LeftToRight, RightToLeft };

enum UnaryOperatorPlace {
    prefix = 0b01,
    postfix = 0b10,
    All = prefix | postfix
};

struct BinaryOperatorInfo {
    int priority;
    Assoc assoc;
};

struct UnaryOperatorInfo {
    int priority;
    UnaryOperatorPlace place;
    Assoc assoc;
};

namespace XSharp {
//{operator, {priority,associativity}}
static std::unordered_map<XString, BinaryOperatorInfo> binaryOperInfo = {
    {"+", {120, LeftToRight}},  {"-", {120, LeftToRight}},
    {"*", {130, LeftToRight}},  {"/", {130, LeftToRight}},
    {"^", {80, LeftToRight}},   {"&", {90, LeftToRight}},
    {"<<", {110, LeftToRight}}, {">>", {110, LeftToRight}},
    {"%", {130, LeftToRight}},  {"!=", {90, LeftToRight}},
    {"=", {50, RightToLeft}},   {"==", {90, LeftToRight}},
    {"+=", {50, RightToLeft}},  {"-=", {50, RightToLeft}},
    {"*=", {50, RightToLeft}},  {"/=", {50, RightToLeft}},
    {"<<=", {50, RightToLeft}}, {">>=", {50, RightToLeft}},
    {"%=", {50, RightToLeft}},  {"&=", {50, RightToLeft}},
    {"|=", {50, RightToLeft}},  {"^=", {50, RightToLeft}},
    {"||", {60, LeftToRight}},  {"&&", {70, LeftToRight}},
    {"=", {50, RightToLeft}},   {"<", {100, LeftToRight}},
    {">", {100, LeftToRight}},  {"<=", {100, LeftToRight}},
    {">=", {100, LeftToRight}}, {"?", {60, LeftToRight}},
    {":", {60, LeftToRight}},
};

static std::unordered_map<XString, UnaryOperatorInfo> unaryOperInfo = {
    {"+",
     {3, prefix, RightToLeft}},  //{operator,{priority,place,associativity}}
    {"-", {3, prefix, RightToLeft}},
    {"++", {3, All, RightToLeft}},
    {"--", {3, All, RightToLeft}},
    {"!", {3, prefix, RightToLeft}}};

}  // namespace XSharp
