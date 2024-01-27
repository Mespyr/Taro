# Ochre 
Ochre is a stack-based programming language with a combination of both high-level and low-level features. It aims to have the simplicity of a Forth-like language, while still possessing the modern features that allow an ease of development.
[Language Specification (Very Much in Progress)](SPECS.md) 

### TODO??

- [x] Ochre will compile programs down to assembly and use the `fasm` assembler to make an executable. This will make the programs many times faster than the interpreter.
- [x] Ochre will have actual string pointers
- [x] labels will be allowed to jump to from anywhere within a function, making control flow easier
- [x] syscalls will be implemented, making the language be able to interact with the operating system
- [x] Basic stack manipulation
- [x] Function arguments and return values
- [x] Type Checking
- [x] have more fancy type notation (using `^` to indicate pointers, making custom types possible)
- [x] Primitive types `Int` and `Char`
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
- [x] allow modifying values stored at pointers on the stack
	- [x] access members of structs pushed onto the stack
	- [x] write and read values to pointers of primitive types
- [x] Constants (const keyword)
- [x] Importing files
- [ ] remove labels for more conventional control flow (while loops and conditionals)
- [ ] switch expressions (sorta)
- [ ] type casting
- [ ] unsafe and safe mode
    - [ ] allow for addition on pointers in unsafe mode, which produces an 'unsafe pointer'
	- [ ] allow for unsafe pointer to be used as a type in variables, struct members and function signatures
	- [ ] unsafe pointer can be on its own (ie: a malloc implementation can return an 'unsafe pointer' with no type)
- [ ] write code to generate a [prime spiral](https://mathimages.swarthmore.edu/index.php/Prime_spiral_(Ulam_spiral)) also known as a Ulam spiral.
