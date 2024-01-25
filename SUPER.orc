struct String
  i64 size
  ^i8 data

fun new_String(i64 ^i8) [^String]
  <String> swp over @String.data
  swp over @String.size

const SYS_write 1
const STDOUT 1

fun print(^String)
  dup &String.size
  swp &String.data
  STDOUT SYS_write call3 pop

const COMPLEX
  34 35 + 21 20 *
  9 + * 10 - 21 / +

fun discover(i64) [i64]
  dup dup + +

fun make(i64) [i64]
  dup discover
  swp dup + / +
  
fun main()
  ^String str
  "Hello World!\n" new_String @str

  &str print
  &str print
  &str print

  COMPLEX discover make dump