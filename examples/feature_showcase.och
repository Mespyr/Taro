struct String
  Int size
  ^Char data

fun new_String(Int ^Char) [^String]
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

fun discover(Int) [Int]
  dup dup + +

fun make(Int) [Int]
  dup discover
  swp dup + / +

fun TEST_PRIM_VARS()
  <Int> dup 12 swp @Int
  &Int dump

fun main()
  ^String str
  "Hello World!\n" new_String @str

  &str print
  &str print
  &str print

  COMPLEX discover make dump
  TEST_PRIM_VARS