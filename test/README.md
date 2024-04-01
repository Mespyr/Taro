# Testing Compilation Target Ideas

### main.c
In this directory, I have a `main.c` and `lib.c`. `main.c` is an example of what a compiled Ochre program would like in C form. It is sectioned as so:

- **Boilerplate**: This section describes all builtin types as well as defining the underlying mechanisms of Ochre.
  
  - The first section is a block of `typedef`s which convert all Ochre's builtin types to their C equivalent.
  
  - The next section describes a union called the `FloatPasser` which helps push floating point numbers onto the stack. Floating points cannot be cast to a `void*`, so we use the union to read the floating point as an integer and cast that as a `void*`. This allows us to indirectly cast between `void*` and `float/double`.
  
  - `OCH_stack` is the global stack that the program runs from and is initialized as an array of `void*`. This means that values on the stack have a size limit of 8 bytes, meaning structs have to be pushed onto the stack via pointer. When values are pushed onto the stack, they are cast to `void*` or passed through the `FloatPasser`.
  
  - `OCH_top` is the index to the top of the stack. It always points 1 ahead from the top value of the stack. When `OCH_top` equals 0, there is nothing on the stack. To push something onto the stack, you first set `OCH_stack[OCH_top]` to a value, and then increment `OCH_top`. This can be simplified down to `OCH_stack[OCH_top++] = ...`, which does everything in one line. To pop off a value from the stack, you first decrement `OCH_top`, and then take the value from `OCH_stack[OCH_top]`. This can be simplified down to `OCH_stack[--OCH_top]` if you are trying to get the value from the top of the stack. If you are just trying to remove the top value without doing anything else with it, just saying `OCH_top--` works.

- **Types**: This section describes all the user-defined types in Ochre.

- **External**: This section describes all external C functions used in the program. Only functions can be external. External C types must be defined in Ochre regularly.
  - When passing structs, remember to pass structs by pointer. The external functions you specify must take pointers to the struct as arguments rather than passing by value.

- **Functions**: This section compiles all of the functions defined in the program to their C equivalent. 

### lib.c
`lib.c` is our external C file which we will link to the object file produced by the compiler. It defines the following functions:

- `void PUTU(I64)`: prints an integer to standard output using printf
- `void PUTD(F64)`: writes a floating point to standard output using printf
- `void print(String)`: prints a string to standard output using printf
- `F32 distance(Vec2*, Vec2*)`: calculates the distance between two points (vectors)

It is important to note that if you want to interface with Ochre types, you also have to define them in your C file (This doesn't apply to primitive types such as I32, I64, U64, U32, etc. which have their own C equivalent).
- `String` is a type-alias to `U8*` or `char*` so in `lib.c`, we also define `String` as such.

While it is possible to call Ochre functions from within `lib.c`, it is not recommended as it could mess with the stack.

### Compilation Process (frontend)
The plan for using the compiler is simple. The default behavior of the compiler is to take the input program, and output an object file. It would be used like this:
```
ochre input.och
```
This would creates an object file `input.o` which can be linked to other object files and libraries using `gcc` or other C compilers.

Because of Ochre's lack of header files and its use of the `import` keyword, compiling two source files together is an unnecesary feature, since you can just import one of the files directly into your main source file. The `extern` keyword is also meant for functions which follow the C calling convention so you can't link two Ochre object files together.
