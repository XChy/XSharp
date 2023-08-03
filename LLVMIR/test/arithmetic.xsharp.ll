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

define void @main() {
entry:
  %n = alloca i32, align 4
  %0 = call i32 @inputI32()
  store i32 %0, ptr %n, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 1, ptr %b, align 4
  %c = alloca i32, align 4
  store i32 1, ptr %c, align 4
  %d = alloca i8, align 1
  store i8 0, ptr %d, align 1
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %n, align 4
  %3 = icmp slt i32 %1, %2
  br i1 %3, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %4 = load i32, ptr %a, align 4
  %5 = sub i32 0, %4
  %6 = load i32, ptr %b, align 4
  %7 = load i32, ptr %c, align 4
  %8 = mul i32 %6, %7
  %9 = sext i32 %8 to i64
  %10 = sdiv i64 %9, 66
  %11 = sext i32 %5 to i64
  %12 = add i64 %11, %10
  %13 = trunc i64 %12 to i32
  store i32 %13, ptr %c, align 4
  %14 = load i32, ptr %c, align 4
  store i32 %14, ptr %a, align 4
  %15 = load i32, ptr %b, align 4
  %16 = sext i32 %15 to i64
  %17 = srem i64 %16, 3
  %18 = sdiv i64 %17, 2
  %19 = add i64 %18, 4
  store i32 4, ptr %b, align 4
  %20 = load i32, ptr %b, align 4
  %21 = sext i32 %20 to i64
  %22 = add i64 %19, %21
  %23 = trunc i64 %22 to i32
  store i32 %23, ptr %a, align 4
  %24 = load i32, ptr %c, align 4
  store i32 %24, ptr %b, align 4
  %25 = load i32, ptr %i, align 4
  %26 = sext i32 %25 to i64
  %27 = add i64 %26, 1
  %28 = trunc i64 %27 to i32
  store i32 %28, ptr %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %29 = load i32, ptr %a, align 4
  %30 = call i8 @printI32(i32 %29)
  %31 = load i32, ptr %b, align 4
  %32 = call i8 @printI32(i32 %31)
  %33 = load i32, ptr %c, align 4
  %34 = call i8 @printI32(i32 %33)
  %35 = load i32, ptr %c, align 4
  %36 = sub i32 0, %35
  %37 = call i8 @printI32(i32 %36)
  %38 = load i32, ptr %c, align 4
  %39 = sext i32 %38 to i64
  %40 = icmp eq i64 %39, -1
  %41 = xor i1 %40, true
  %42 = call i8 @printBoolean(i1 %41)
  ret void
}
