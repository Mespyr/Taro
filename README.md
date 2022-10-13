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
	- [x] struct definition
	- [x] struct definition as variable
	- [x] setting and reading struct members
	- [x] compile structs inside of other structs and write structs into them
	- [x] passing structs into functions
	- [ ] setting struct variables from other defined structs
- [ ] Variables
	- [x] making struct veriables
	- [ ] setting variable types to primitive types
- [ ] Arrays
- [ ] write code to generate a [prime spiral](https://mathimages.swarthmore.edu/index.php/Prime_spiral_(Ulam_spiral)) also known as a Ulam spiral.


### Variables
Defining a variable will require a type and a name, like so
```python
fun main()
  i64 count
  StructName struct_
end
```
To push a pointer to the variable onto the stack, put the variable name onto the stack
```
fun main()
  i64 count
  count # pointer to i64 (^i64)

  String s
  s # pushes pointer to String (^String)
```

To set the value as anything that is it's type, use the `@` key
```python
fun main()
  i64 count
  69 @count # set count to 69
end
```

If the variable is a base type (i8, i64) or is a pointer to those types, you can read from them like this:
```python
fun main()
  i64 count
  &count # read value of count
end
```

If you want to copy a variable's value into a new variable, it needs to be written like this
Value must be a pointer to the variables type.
```python
fun main()
  i64 count
  i64 count2

  12 @count # set count to 12
  count $count2 # set count2 to count
end
```

This is useful if you want to pass a struct into a function and set is a variable to access its members
```python
fun print(^String)
  String s $s # get top value and set it to s
end

fun main()
  String str
  str print
end
```

### Structs
LCP will have structs which will store data into sections kind of like this:
```python
struct EpicStruct
  i64 num1
  i64 num2
end
```

and can be set as a type for a variable like this:
```python
fun main()
  EpicStruct epic_name_for_struct
end
```

You cannot read the struct variable directly using `&epic_name_for_struct` you must reference one of its types like so:
```python
fun main()
  EpicStruct epic_name_for_struct
  &epic_name_for_struct.num1 # read struct member num1
  &epic_name_for_struct.num2 # read struct member num2
end
```

You can write directly to the variable using the `@` intrinsic but you can also write to its members
```python
fun main()
  EpicStruct epic_name_for_struct
  EpicStruct copy_of_var

  epic_name_for_struct @copy_of_var  # write the epic_name_for_struct to copy_of_var
  12 @epic_name_for_struct.num1      # set epic_name_for_struct.num1 to 12
  13 @epic_name_for_struct.num2      # set epic_name_for_struct.num2 to 13
end
```
