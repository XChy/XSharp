; ModuleID = 'XSharp'
source_filename = "XSharp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

declare i8 @printI32(i32)

declare i8 @printI64(i64)

declare i8 @printChar(i16)

declare i8 @printStr(ptr)

declare i8 @printBoolean(i1)

declare i8 @printDouble(double)

declare i32 @inputI32()

define i64 @add(i32 %a, i32 %b) {
entry:
  %0 = alloca i32, align 4
  store i32 %a, ptr %0, align 4
  %1 = alloca i32, align 4
  store i32 %b, ptr %1, align 4
  %2 = load i32, ptr %0, align 4
  %3 = load i32, ptr %1, align 4
  %4 = add i32 %2, %3
  %5 = sext i32 %4 to i64
  ret i64 %5
}

define i32 @main() {
entry:
  %0 = call i8 @printI64(i64 2)
  %1 = call i8 @printDouble(double 3.100000e+00)
  %2 = call i8 @printDouble(double 0x3FFD1745D1745D17)
  %3 = call i8 @printBoolean(i1 true)
  ret i32 21
}
