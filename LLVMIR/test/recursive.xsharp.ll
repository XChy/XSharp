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

define i32 @fib(i32 %n) {
entry:
  %0 = alloca i32, align 4
  store i32 %n, ptr %0, align 4
  %1 = load i32, ptr %0, align 4
  %2 = sext i32 %1 to i64
  %3 = icmp sle i64 %2, 0
  br i1 %3, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  ret i32 -3

if.else:                                          ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %if.else
  %4 = load i32, ptr %0, align 4
  %5 = sext i32 %4 to i64
  %6 = icmp eq i64 %5, 1
  %7 = load i32, ptr %0, align 4
  %8 = sext i32 %7 to i64
  %9 = icmp eq i64 %8, 2
  %10 = or i1 %6, %9
  br i1 %10, label %if.then1, label %if.else2

if.then1:                                         ; preds = %if.end
  ret i32 1

if.else2:                                         ; preds = %if.end
  br label %if.end3

if.end3:                                          ; preds = %if.else2
  %11 = load i32, ptr %0, align 4
  %12 = sext i32 %11 to i64
  %13 = sub i64 %12, 1
  %14 = trunc i64 %13 to i32
  %15 = call i32 @fib(i32 %14)
  %16 = load i32, ptr %0, align 4
  %17 = sext i32 %16 to i64
  %18 = sub i64 %17, 2
  %19 = trunc i64 %18 to i32
  %20 = call i32 @fib(i32 %19)
  %21 = add i32 %15, %20
  ret i32 %21
}

define i32 @main() {
entry:
  %0 = call i32 @inputI32()
  %1 = call i32 @fib(i32 %0)
  %2 = call i8 @printI32(i32 %1)
  ret i32 3
}
