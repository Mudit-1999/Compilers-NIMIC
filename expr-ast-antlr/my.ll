; ModuleID = 'my compiler'
source_filename = "my compiler"

@a = global i32 3
@b = global i8 0

define i8 @asw(i32 %a, i8 %b) {
asw_entry:
  %b2 = alloca i8
  %a1 = alloca i32
  store i32 %a, i32* %a1
  store i8 %b, i8* %b2
  ret i8 97
}

define void @abc() {
abc_entry:
  ret void
}

define i32 @main() {
main_entry:
  %0 = load i32, i32* @a
  %1 = load i32, i32* @a
  %add = add i32 %1, 4
  %add1 = add i32 %add, 3
  ret i32 0
}


IO

Done
ArrayID
FunctionCall
Type checking 
For loop 
while loop 