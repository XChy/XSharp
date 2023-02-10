#pragma once

#include "xsharp_global.h"
#include "XString.h"
// Assocativity
enum Assoc { LeftToRight, RightToLeft };

enum UnaryOperatorPlace {
    BeforeValue = 0b01,
    AfterValue = 0b10,
    All = BeforeValue | AfterValue
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
    {"+", {5, LeftToRight}},    {"-", {5, LeftToRight}},
    {"*", {4, LeftToRight}},    {"/", {4, LeftToRight}},
    {"^", {11, LeftToRight}},   {"&", {10, LeftToRight}},
    {"%", {4, LeftToRight}},    {"!=", {9, LeftToRight}},
    {"=", {16, RightToLeft}},   {"==", {9, LeftToRight}},
    {"+=", {16, RightToLeft}},  {"-=", {16, RightToLeft}},
    {"*=", {16, RightToLeft}},  {"/=", {16, RightToLeft}},
    {"<<=", {16, RightToLeft}}, {">>=", {16, RightToLeft}},
    {"%=", {16, RightToLeft}},  {"&=", {16, RightToLeft}},
    {"|=", {16, RightToLeft}},  {"^=", {16, RightToLeft}},
    {"=", {16, RightToLeft}},   {"<", {8, LeftToRight}},
    {">", {8, LeftToRight}},    {"<=", {8, LeftToRight}},
    {">=", {8, LeftToRight}},   {"?", {15, LeftToRight}},
    {":", {15, LeftToRight}},
};

static std::unordered_map<XString, UnaryOperatorInfo> unaryOperInfo = {
    {"+",
     {3, BeforeValue,
      RightToLeft}},  //{operator,{priority,place,associativity}}
    {"-", {3, BeforeValue, RightToLeft}},
    {"++", {3, All, RightToLeft}},
    {"--", {3, All, RightToLeft}},
    {"!", {3, BeforeValue, RightToLeft}}};
}  // namespace XSharp
