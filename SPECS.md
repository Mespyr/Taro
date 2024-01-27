# Ochre Language Specification
This document introduces the reader to the Ochre Programming Language as well as document all of its features.

## The Stack
The stack is the main place the programmer will manage and manipulate data from. It follows the Last In, First Out (LIFO) Principle, where last value pushed onto the stack will be the first to be popped off.
In Ochre, values that are pushed onto the stack must be either a Primitive Type, or must be a pointer to a structure.
```
fun main()
  2 3 4 5  # pushing four values onto the stack
```
In this example, you can see we push four numbers onto the stack. The stack would look something like `[2, 3, 4, 5]` if the right side was the top of the stack. If we popped the top value off, the stack would look like `[2, 3, 4]`. 

## Functions
To execute code, a main function is required. To define a function, you must use the `fun` keyword like so:
```
fun my_epic_func(Int Int) [Int]
  ...
```
In this example, we are defining the function `my_epic_func`. Inside the parentheses are the arguments that the function takes. These values are taken from the stack and are put onto the stack of the function. Inside of the braces are the return items. You can return multiple items from a function, and these values are also pushed onto the stack.

The main function in Ochre shouldn't take any arguments or return anything. Trying to pass any arguments into its signature will result in an error.

## Primitive Types
There are two primitive types inside of Ochre. These basic types help create much more complicated data structures.
- `Int`:  A 64-bit integer which will store numbers.
- `Char`: An 8-bit integer which typically stores character data.
These are the only types that are able to be pushed onto the stack as is. Any user-created structs have to be pushed onto the stack as a pointer.

### Strings

## Stack Manipulation
