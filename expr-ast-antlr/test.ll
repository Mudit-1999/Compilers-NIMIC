; ModuleID = 'my compiler'
source_filename = "my compiler"
target triple = "x86_64-pc-linux-gnu"

define i32 @pow(i32 %a, i32 %b) {
pow_entry:
  %i = alloca i32
  %tmp = alloca i32
  %b2 = alloca i32
  %a1 = alloca i32
  store i32 %a, i32* %a1
  store i32 %b, i32* %b2
  store i32 1, i32* %tmp
  br label %startloop

startloop:                                        ; preds = %pow_entry
  store i32 0, i32* %i
  %0 = load i32, i32* %i
  %1 = load i32, i32* %b2
  %lt = icmp slt i32 %0, %1
  br i1 %lt, label %loop, label %afterloop

loop:                                             ; preds = %updateloop, %startloop
  %2 = load i32, i32* %tmp
  %3 = load i32, i32* %a1
  %mult = mul i32 %2, %3
  store i32 %mult, i32* %tmp
  br label %updateloop

updateloop:                                       ; preds = %loop
  %4 = load i32, i32* %i
  %add = add i32 %4, 1
  store i32 %add, i32* %i
  %5 = load i32, i32* %i
  %6 = load i32, i32* %b2
  %lt3 = icmp slt i32 %5, %6
  br i1 %lt3, label %loop, label %afterloop

afterloop:                                        ; preds = %updateloop, %startloop
  %7 = load i32, i32* %tmp
  ret i32 %7
}

define i32 @main() {
main_entry:
  %i = alloca i32
  %sum = alloca i32
  %k = alloca i32
  %N = alloca i32
  br label %startloop

startloop:                                        ; preds = %main_entry
  store i32 1, i32* %i
  %0 = load i32, i32* %i
  %1 = load i32, i32* %N
  %le = icmp sle i32 %0, %1
  br i1 %le, label %loop, label %afterloop

loop:                                             ; preds = %updateloop, %startloop
  %2 = load i32, i32* %sum
  %3 = load i32, i32* %i
  %4 = load i32, i32* %k
  %5 = call i32 @pow(i32 %3, i32 %4)
  %add = add i32 %2, %5
  store i32 %add, i32* %sum
  br label %updateloop

updateloop:                                       ; preds = %loop
  %6 = load i32, i32* %i
  %add1 = add i32 %6, 1
  store i32 %add1, i32* %i
  %7 = load i32, i32* %i
  %8 = load i32, i32* %N
  %le2 = icmp sle i32 %7, %8
  br i1 %le2, label %loop, label %afterloop

afterloop:                                        ; preds = %updateloop, %startloop
  ret i32 0
}
