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

define i1 @isPrime(i32 %x) {
entry:
  %0 = alloca i32, align 4
  store i32 %x, ptr %0, align 4
  %1 = load i32, ptr %0, align 4
  %2 = sext i32 %1 to i64
  %3 = icmp sle i64 %2, 1
  br i1 %3, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  ret i1 false

if.else:                                          ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %if.else
  %i = alloca i32, align 4
  store i32 2, ptr %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %if.end3, %if.end
  %4 = load i32, ptr %i, align 4
  %5 = load i32, ptr %i, align 4
  %6 = mul i32 %4, %5
  %7 = load i32, ptr %0, align 4
  %8 = icmp sle i32 %6, %7
  br i1 %8, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %9 = load i32, ptr %0, align 4
  %10 = load i32, ptr %i, align 4
  %11 = srem i32 %9, %10
  %12 = sext i32 %11 to i64
  %13 = icmp eq i64 %12, 0
  br i1 %13, label %if.then1, label %if.else2

if.then1:                                         ; preds = %while.body
  ret i1 false

if.else2:                                         ; preds = %while.body
  br label %if.end3

if.end3:                                          ; preds = %if.else2
  %14 = load i32, ptr %i, align 4
  %15 = sext i32 %14 to i64
  %16 = add i64 %15, 1
  %17 = trunc i64 %16 to i32
  store i32 %17, ptr %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret i1 true
}

define i32 @main() {
entry:
  %a = alloca i1, align 1
  store i1 true, ptr %a, align 1
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i1, ptr %a, align 1
  br i1 %0, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %x = alloca i32, align 4
  %1 = call i32 @inputI32()
  store i32 %1, ptr %x, align 4
  %2 = load i32, ptr %x, align 4
  %3 = call i1 @isPrime(i32 %2)
  %4 = call i8 @printBoolean(i1 %3)
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret i32 32
}
