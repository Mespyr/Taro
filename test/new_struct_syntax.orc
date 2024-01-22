struct String
  i64 size
  ^i8 data
struct Object
  i64 int_val
  String str_val

fun String_init(i64 ^i8) [^String]
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
