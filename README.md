# lcp
### The Lipsem Compiler Project

This is a remake of the [Lipsem programming language](https://github.com/Mespyr/lipsem) as a compiler instead of an interpreter.
Most of the lexer uses code from the [Ach programming language](https://github.com/Mespyr/ach).
*(both of these languages were created by me)*

### Improvements and Changes

[*] LCP will compile programs down to assembly and use the `fasm` assembler to make an executable. This will make the programs many times faster than the interpreter.
[] LCP will have actual string pointers
[*] labels will be allowed to jump to from anywhere within a function, making control flow easier
[] syscalls will be implemented, making the language be able to interact with the operating system
[] keywords and language design will be changed to improve readability
