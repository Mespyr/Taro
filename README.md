# Ochre 
Ochre is a stack-based programming language with a combination of both high-level and low-level features. It aims to have the simplicity of a Forth-like language, while still possessing the modern features that allow an ease of development.
[Language Specification (Very Much in Progress)](SPECS.md) 

## TODO??
- [ ] Finish assembly optimizations
- [ ] Cond (if/switch statement)
```
fun main()
  Int x 12 @x
  cond
  | &x 12 = : 13 @x ;
  | &x 14 = : 12 @x ;

  &x dump
```

- [ ] While Loops
```
fun main()
  Int idx 0 @idx
  while &idx 10 < :
    &idx dup dump
    1 + @idx ;
```

- [ ] Type Cast
```
fun neg(Int) {Int}
  dup dup + -

fun make_smth_idk(Int) {^Char}
  # mmap syscall
  0 swp # item on stack is size
  3 33 1 neg 0
  9 call6
  cast ^Char # syscalls push integer, so cast to ^Char
```

- [ ] Generics
```
fun alloc[T](Int) {T}
  # mmap syscall
  0 swp
  3 33 1 neg 0
  9 call6 cast T

fun main()
  ^Char str
  20 alloc[^Char] @str
```
