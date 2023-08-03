; ModuleID = 'XSharp'
source_filename = "XSharp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@0 = constant [9 x i16] [i16 72, i16 44, i16 119, i16 111, i16 114, i16 108, i16 100, i16 33, i16 10]
@1 = constant { i64, ptr } { i64 9, ptr @0 }
@2 = constant [9 x i16] [i16 72, i16 44, i16 119, i16 111, i16 114, i16 108, i16 100, i16 33, i16 10]
@3 = constant { i64, ptr } { i64 9, ptr @2 }

declare i8 @printI32(i32)

declare i8 @printI64(i64)

declare i8 @printChar(i16)

declare i8 @printStr(ptr)

declare i8 @printBoolean(i1)

declare i8 @printDouble(double)

declare i32 @inputI32()

define i16 @charAt(ptr %self, i64 %index) {
entry:
  %self.data = getelementptr inbounds { i64, ptr }, ptr %self, i64 0, i32 1
  %0 = load ptr, ptr %self.data, align 8
  %1 = getelementptr inbounds { i64, ptr }, ptr %0, i64 0, i32 1
  %2 = load ptr, ptr %1, align 8
  %"index in self.data" = getelementptr i16, ptr %2, i64 %index
  %3 = load i16, ptr %"index in self.data", align 2
  ret i16 %3
}

define ptr @getData(ptr %self) {
entry:
  %self.data = getelementptr inbounds { i64, ptr }, ptr %self, i64 0, i32 1
  %0 = load ptr, ptr %self.data, align 8
  %1 = load i64, ptr %0, align 8
  %2 = tail call i8 @printI32(i64 %1)
  %3 = load ptr, ptr %self.data, align 8
  ret ptr %3
}

define ptr @"string:new"(ptr %chars) {
entry:
  %0 = tail call { i64, ptr } @GC_new_object(i64 16)
  %.elt = extractvalue { i64, ptr } %0, 0
  %.cast = inttoptr i64 %.elt to ptr
  %self.data = getelementptr inbounds { i64, ptr }, ptr %.cast, i64 0, i32 1
  %1 = load i64, ptr %chars, align 8
  %2 = shl i64 %1, 1
  %3 = alloca { i64, ptr }, align 8
  %4 = tail call ptr @GC_new_object(i64 %2)
  %5 = insertelement <2 x i64> poison, i64 %1, i32 0
  %6 = ptrtoint ptr %4 to i64
  %7 = insertelement <2 x i64> %5, i64 %6, i32 1
  store <2 x i64> %7, ptr %3, align 8
  store ptr %3, ptr %self.data, align 8
  %8 = getelementptr inbounds { i64, ptr }, ptr %chars, i64 0, i32 1
  br label %while.cond

while.cond:                                       ; preds = %Flow, %entry
  %i.0 = phi i64 [ 0, %entry ], [ %17, %Flow ]
  %9 = load i64, ptr %chars, align 8
  %10 = icmp slt i64 %i.0, %9
  br i1 %10, label %while.body, label %Flow

while.body:                                       ; preds = %while.cond
  %11 = load ptr, ptr %self.data, align 8
  %12 = getelementptr inbounds { i64, ptr }, ptr %11, i64 0, i32 1
  %13 = load ptr, ptr %12, align 8
  %"i in self.data" = getelementptr i16, ptr %13, i64 %i.0
  %14 = load ptr, ptr %8, align 8
  %"i in chars" = getelementptr i16, ptr %14, i64 %i.0
  %15 = load i16, ptr %"i in chars", align 2
  store i16 %15, ptr %"i in self.data", align 2
  %16 = add i64 %i.0, 1
  br label %Flow

Flow:                                             ; preds = %while.body, %while.cond
  %17 = phi i64 [ %16, %while.body ], [ undef, %while.cond ]
  br i1 %10, label %while.cond, label %while.end

while.end:                                        ; preds = %Flow
  ret ptr %.cast
}

declare { i64, ptr } @GC_new_object(i64)

define i32 @main() {
entry:
  %0 = alloca { i64, ptr }, align 8
  %a = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(i64 16, ptr %0)
  %1 = call { i64, ptr } @GC_new_object(i64 16)
  %.elt.i = extractvalue { i64, ptr } %1, 0
  %.cast.i = inttoptr i64 %.elt.i to ptr
  %self.data.i = getelementptr inbounds { i64, ptr }, ptr %.cast.i, i64 0, i32 1
  %2 = call ptr @GC_new_object(i64 18)
  %3 = ptrtoint ptr %2 to i64
  %4 = insertelement <2 x i64> <i64 9, i64 poison>, i64 %3, i32 1
  store <2 x i64> %4, ptr %0, align 8
  store ptr %0, ptr %self.data.i, align 8
  br label %while.cond.i

while.cond.i:                                     ; preds = %Flow.i, %entry
  %i.0.i = phi i64 [ 0, %entry ], [ %11, %Flow.i ]
  %5 = icmp slt i64 %i.0.i, 9
  br i1 %5, label %while.body.i, label %Flow.i

while.body.i:                                     ; preds = %while.cond.i
  %6 = load ptr, ptr %self.data.i, align 8
  %7 = getelementptr inbounds { i64, ptr }, ptr %6, i64 0, i32 1
  %8 = load ptr, ptr %7, align 8
  %"i in self.data.i" = getelementptr i16, ptr %8, i64 %i.0.i
  %"i in chars.i" = getelementptr i16, ptr @0, i64 %i.0.i
  %9 = load i16, ptr %"i in chars.i", align 2
  store i16 %9, ptr %"i in self.data.i", align 2
  %10 = add i64 %i.0.i, 1
  br label %Flow.i

Flow.i:                                           ; preds = %while.body.i, %while.cond.i
  %11 = phi i64 [ %10, %while.body.i ], [ undef, %while.cond.i ]
  br i1 %5, label %while.cond.i, label %"string:new.exit"

"string:new.exit":                                ; preds = %Flow.i
  call void @llvm.lifetime.end.p0(i64 16, ptr %0)
  store ptr %.cast.i, ptr %a, align 8
  %12 = call i8 @printChar(i16 44)
  %13 = load ptr, ptr %a, align 8
  %self.data.i1 = getelementptr inbounds { i64, ptr }, ptr %13, i64 0, i32 1
  %14 = load ptr, ptr %self.data.i1, align 8
  %15 = load i64, ptr %14, align 8
  %16 = call i8 @printI32(i64 %15)
  %17 = load ptr, ptr %self.data.i1, align 8
  %18 = call i8 @printStr(ptr %17)
  %19 = load ptr, ptr %a, align 8
  %self.data.i2 = getelementptr inbounds { i64, ptr }, ptr %19, i64 0, i32 1
  %20 = load ptr, ptr %self.data.i2, align 8
  %21 = load i64, ptr %20, align 8
  %22 = call i8 @printI32(i64 %21)
  %23 = load ptr, ptr %self.data.i2, align 8
  %24 = getelementptr inbounds { i64, ptr }, ptr %23, i32 0, i32 1
  %25 = load ptr, ptr %24, align 8
  %"3 in call a.getData()" = getelementptr i16, ptr %25, i64 3
  store i16 36, ptr %"3 in call a.getData()", align 2
  %26 = load ptr, ptr %a, align 8
  %self.data.i3 = getelementptr inbounds { i64, ptr }, ptr %26, i64 0, i32 1
  %27 = load ptr, ptr %self.data.i3, align 8
  %28 = getelementptr inbounds { i64, ptr }, ptr %27, i64 0, i32 1
  %29 = load ptr, ptr %28, align 8
  %"index in self.data.i" = getelementptr i16, ptr %29, i64 3
  %30 = load i16, ptr %"index in self.data.i", align 2
  %31 = call i8 @printChar(i16 %30)
  ret i32 0
}

; Function Attrs: argmemonly nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #0

; Function Attrs: argmemonly nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #0

attributes #0 = { argmemonly nocallback nofree nosync nounwind willreturn }
