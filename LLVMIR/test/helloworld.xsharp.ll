; ModuleID = 'XSharp'
source_filename = "XSharp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@0 = constant [12 x i16] [i16 72, i16 101, i16 108, i16 108, i16 111, i16 44, i16 87, i16 111, i16 114, i16 108, i16 100, i16 33]
@1 = constant { i64, ptr } { i64 12, ptr @0 }

declare i8 @printI32(i32)

declare i8 @printI64(i64)

declare i8 @printChar(i16)

declare i8 @printStr(ptr)

declare i8 @printBoolean(i1)

declare i8 @printDouble(double)

declare i32 @inputI32()

define void @main() {
entry:
  %0 = call i8 @printStr(ptr @1)
  ret void
}
