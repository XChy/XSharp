; ModuleID = 'XSharp'
source_filename = "XSharp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@0 = constant [15 x i16] [i16 65, i16 118, i16 101, i16 114, i16 97, i16 103, i16 101, i16 32, i16 97, i16 103, i16 101, i16 32, i16 105, i16 115, i16 58]
@1 = constant { i64, ptr } { i64 15, ptr @0 }

declare i8 @printI32(i32)

declare i8 @printI64(i64)

declare i8 @printChar(i16)

declare i8 @printStr(ptr)

declare i8 @printBoolean(i1)

declare i8 @printDouble(double)

declare i32 @inputI32()

define void @setAge(ptr %self, i32 %age) {
entry:
  %self.age = getelementptr inbounds { i64, i32 }, ptr %self, i64 0, i32 1
  store i32 %age, ptr %self.age, align 4
  ret void
}

define i32 @getAge(ptr %self) {
entry:
  %self.age = getelementptr inbounds { i64, i32 }, ptr %self, i64 0, i32 1
  %0 = load i32, ptr %self.age, align 4
  ret i32 %0
}

define ptr @"Foo:new"() {
entry:
  %0 = tail call { i64, i32 } @GC_new_object(i64 12)
  %.fca.0.extract = extractvalue { i64, i32 } %0, 0
  %1 = inttoptr i64 %.fca.0.extract to ptr
  %self.age = getelementptr inbounds { i64, i32 }, ptr %1, i64 0, i32 1
  store i32 0, ptr %self.age, align 4
  ret ptr %1
}

declare { i64, i32 } @GC_new_object(i64)

define ptr @"Foo:new.1"(i32 %a) {
entry:
  %0 = tail call { i64, i32 } @GC_new_object(i64 12)
  %.fca.0.extract = extractvalue { i64, i32 } %0, 0
  %1 = inttoptr i64 %.fca.0.extract to ptr
  %self.age = getelementptr inbounds { i64, i32 }, ptr %1, i64 0, i32 1
  store i32 %a, ptr %self.age, align 4
  ret ptr %1
}

define i32 @main() {
entry:
  %n = alloca i64, align 8
  %0 = call i32 @inputI32()
  %1 = sext i32 %0 to i64
  store i64 %1, ptr %n, align 8
  %c = alloca ptr, align 8
  %2 = load i64, ptr %n, align 8
  %3 = mul i64 %2, 8
  %4 = alloca { i64, ptr }, align 8
  %5 = call ptr @GC_new_object(i64 %3)
  %6 = getelementptr inbounds { i64, ptr }, ptr %4, i32 0, i32 0
  %7 = getelementptr inbounds { i64, ptr }, ptr %4, i32 0, i32 1
  store i64 %2, ptr %6, align 8
  store ptr %5, ptr %7, align 8
  store ptr %4, ptr %c, align 8
  %i = alloca i64, align 8
  %8 = load i64, ptr %n, align 8
  store i64 %8, ptr %i, align 8
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %9 = load i64, ptr %i, align 8
  %10 = sub i64 %9, 1
  %11 = icmp sge i64 %10, 0
  br i1 %11, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %12 = load i64, ptr %i, align 8
  %13 = sub i64 %12, 1
  %14 = load ptr, ptr %c, align 8
  %15 = getelementptr inbounds { i64, ptr }, ptr %14, i32 0, i32 1
  %16 = load ptr, ptr %15, align 8
  %"[i] - [1] in c" = getelementptr ptr, ptr %16, i64 %13
  %17 = call i32 @inputI32()
  %18 = call { i64, i32 } @GC_new_object(i64 12)
  %.fca.0.extract.i = extractvalue { i64, i32 } %18, 0
  %19 = inttoptr i64 %.fca.0.extract.i to ptr
  %self.age.i = getelementptr inbounds { i64, i32 }, ptr %19, i64 0, i32 1
  store i32 %17, ptr %self.age.i, align 4
  store ptr %19, ptr %"[i] - [1] in c", align 8
  %20 = load i64, ptr %i, align 8
  %21 = sub i64 %20, 1
  store i64 %21, ptr %i, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %sum = alloca double, align 8
  store double 0.000000e+00, ptr %sum, align 8
  %22 = load i64, ptr %n, align 8
  store i64 %22, ptr %i, align 8
  br label %while.cond1

while.cond1:                                      ; preds = %while.body2, %while.end
  %23 = load i64, ptr %i, align 8
  %24 = sub i64 %23, 1
  %25 = icmp sge i64 %24, 0
  br i1 %25, label %while.body2, label %while.end4

while.body2:                                      ; preds = %while.cond1
  %26 = load double, ptr %sum, align 8
  %27 = load i64, ptr %i, align 8
  %28 = sub i64 %27, 1
  %29 = load ptr, ptr %c, align 8
  %30 = getelementptr inbounds { i64, ptr }, ptr %29, i32 0, i32 1
  %31 = load ptr, ptr %30, align 8
  %"[i] - [1] in c3" = getelementptr ptr, ptr %31, i64 %28
  %32 = load ptr, ptr %"[i] - [1] in c3", align 8
  %self.age.i5 = getelementptr inbounds { i64, i32 }, ptr %32, i64 0, i32 1
  %33 = load i32, ptr %self.age.i5, align 4
  %34 = sitofp i32 %33 to double
  %35 = fadd double %26, %34
  store double %35, ptr %sum, align 8
  %36 = load i64, ptr %i, align 8
  %37 = sub i64 %36, 1
  store i64 %37, ptr %i, align 8
  br label %while.cond1

while.end4:                                       ; preds = %while.cond1
  %38 = call i8 @printStr(ptr @1)
  %39 = load double, ptr %sum, align 8
  %40 = load i64, ptr %n, align 8
  %41 = sitofp i64 %40 to double
  %42 = fdiv double %39, %41
  %43 = call i8 @printDouble(double %42)
  ret i32 0
}
