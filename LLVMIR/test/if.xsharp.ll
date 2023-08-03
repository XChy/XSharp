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

define i64 @main() {
entry:
  %i = alloca i32, align 4
  %0 = call i32 @inputI32()
  store i32 %0, ptr %i, align 4
  %1 = load i32, ptr %i, align 4
  %2 = sext i32 %1 to i64
  %3 = icmp eq i64 %2, 3
  br i1 %3, label %if.then, label %if.else2

if.then:                                          ; preds = %entry
  %4 = call i8 @printI64(i64 0)
  %5 = load i32, ptr %i, align 4
  %6 = sext i32 %5 to i64
  %7 = icmp sgt i64 %6, 4
  br i1 %7, label %if.then1, label %if.else

if.then1:                                         ; preds = %if.then
  %8 = call i8 @printI64(i64 2)
  br label %if.end

if.else:                                          ; preds = %if.then
  %9 = call i8 @printI64(i64 3)
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then1
  br label %if.end3

if.else2:                                         ; preds = %entry
  %10 = call i8 @printI64(i64 1)
  br label %if.end3

if.end3:                                          ; preds = %if.else2, %if.end
  %11 = call i8 @printI64(i64 12)
  ret i64 10
}
