struct String
  Int size
  ^Char data

fun new_String(Int ^Char) [^String]
  <String> swp over @String.data
  swp over @String.size

fun print(^String)
  dup &String.size
  swp &String.data
  1 1 call3 pop

fun main()
  "Hello World!\n" new_String print
