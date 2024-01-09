# Orc 

Originally started as a remake of the [Lipsem programming language](https://github.com/Mespyr/lipsem), Orc has diverged a lot into a compiled, stack-based programming language.
Source files for Orc end in the `.orc` file extension.

### TODO??

- [x] Orc will compile programs down to assembly and use the `fasm` assembler to make an executable. This will make the programs many times faster than the interpreter.
- [x] Orc will have actual string pointers
- [x] labels will be allowed to jump to from anywhere within a function, making control flow easier
- [x] syscalls will be implemented, making the language be able to interact with the operating system
- [x] Basic stack manipulation
- [x] Function arguments and return values
- [x] Type Checking
- [x] have more fancy type notation (using `^` to indicate pointers, making custom types possible)
- [x] add two types of integer (i8, i64) instead of just default `int` keyword. good for string as string type is a `^i8`.
- [x] Structs
	- [x] struct definition
	- [x] struct definition as variable
	- [x] setting and reading struct members
	- [x] compile structs inside of other structs and write structs into them
	- [x] passing structs into functions
	- [x] setting struct variables from other defined structs
- [x] Variables
	- [x] making struct veriables
	- [x] setting variable types to primitive types
	- [x] changing between setting variable form variable on stack and value on stack (a pointer vs the primitive type)
- [ ] type casting
- [ ] unsafe and safe mode
    - [ ] allow for addition on pointers in unsafe mode, which produces an 'unsafe pointer'
	- [ ] allow for unsafe pointer to be used as a type in variables, struct members and function signatures
	- [ ] unsafe pointer can be on its own (ie: a malloc implementation can return an 'unsafe pointer' with no type)
- [x] allow modifying values stored at pointers on the stack
	- [x] access members of structs pushed onto the stack
	- [x] write and read values to pointers of primitive types
- [x] Constants (const keyword)
- [x] Importing files
- [ ] write code to generate a [prime spiral](https://mathimages.swarthmore.edu/index.php/Prime_spiral_(Ulam_spiral)) also known as a Ulam spiral.
- [ ] remove labels for more conventional control flow (while loops and SEXP)
- [ ] SEXP (switch expressions)


### SEXP Ideal Design
```
fun factorial(i64) [i64]
  sexp
    0 => pop 1,
	_ => dup 1 - factorial *,
  end
end

fun main()
  12 factorial dump
end
```
SEXP works by taking the top integer on the stack, and comparing it to a bunch of different other values. If it is, then it executes that section of code up to the `,`. 
The integer is not consumed by the operation unless removed by the expression, and all expressions must return the same types onto the stack for it to pass with the type checker.
Here's another example
``` 
fun fib(i64) [i64]
  sexp
    0 => pop 1,
    1 => pop 1,
    _ =>
      dup 1 - fib
      swp 2 - fib +,
  end
end
```
