struct String
  Int size
  ^Char data

struct Object
  Int int_val
  String str_val

fun String_init(Int ^Char) [^String]
  <String> swp over @String.data
   swp over @String.size

fun String_print(^String)
  String str @str
  &str.size &str.data 1 1 call3 pop

fun main()
  Object obj
  "Hello World!\n" String_init @obj.str_val
  69 @obj.int_val
  &obj.str_val String_print
