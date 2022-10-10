# lcp
### The Lipsem Compiler Project

This is a remake of the [Lipsem programming language](https://github.com/Mespyr/lipsem) as a compiler instead of an interpreter.
Most of the lexer uses code from the [Ach programming language](https://github.com/Mespyr/ach).
*(both of these languages were created by me)*

At this point, the language is almost completely different from Lipsem other than the labels that you can jump to.
It is still very unreadable though.

### Improvements and Changes

- [x] LCP will compile programs down to assembly and use the `fasm` assembler to make an executable. This will make the programs many times faster than the interpreter.
- [x] LCP will have actual string pointers
- [x] labels will be allowed to jump to from anywhere within a function, making control flow easier
- [x] syscalls will be implemented, making the language be able to interact with the operating system
- [x] Basic stack manipulation
- [x] Function arguments and return values
- [x] Type Checking
- [x] have more fancy type notation (using `^` to indicate pointers, making custom types possible)
- [x] add two types of integer (i8, i64) instead of just default `int` keyword. good for string as string type is a `^i8`.
- [ ] Structs
- [ ] write code to generate a [prime spiral](https://mathimages.swarthmore.edu/index.php/Prime_spiral_(Ulam_spiral)) also known as a Ulam spiral.

### Structs
LCP will have structs which will store data into sections kind of like this:
```python
struct EpicStruct
  i64 num1
  i64 num2
end
```

and would be defined and accessed in the code like this:
```python
fun main()
  EpicStruct epic_name_for_struct

  # set num1 to 12
  12 &epic_name_for_struct.num1
  # set num2 to 8
  8 &epic_name_for_struct.num2

  # grab num1
  @epic_name_for_struct.num1
  # grab num2
  @epic_name_for_struct.num2
  
  + dump
end
```

You could also push structs onto the stack and then set them as a veriable elsewhere, for example, inside of a function

```python
struct String
  i64 size
  ^i8 data
end

fun init_String(i64 ^i8 String)
  set String str
  &str.data
  &str.size
end

fun print(String)
  set String str
  @str.size @str.data 1 1 call3 pop
end


fun main()
  String str
  "Hello, World!" str init_String
  str print
end
```
