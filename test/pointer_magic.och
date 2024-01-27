# program will
# 1. make variable of type String with value "Hello World!\n"
# 2. push the pointer to the variable onto the stack and save that into a variable of type ^String
# 3. push the pointer to the var onto the stack (making ^^String) and calling get_string_in_ptr_ptr.
# 4. get_string_in_ptr_ptr will create a value of type ^String and save ^^String to that
#      (since it is a pointer to a ^String, the compiler interprets it as a pointer to a variable,
#       and allows us to save the contents of the 'var' into the new var)
# 5. read the variable (pushes a ^String)
# 6. save that value to a var of type String and print it

struct String
  Int size
  ^Char data

fun get_string_in_ptr_ptr(^^String) [^String]
	# save the string to s (^^String is pointer to ^String which is type of var s)
	# and read the value (get ^String)
	^String s @s &s

fun print(^String)
	String s @s
	&s.size &s.data 1 1 call3 pop

fun main()
	String s
	"Hello World!\n" @s.data @s.size
	^String str_ptr
	s @str_ptr
	
	String str_2
	str_ptr get_string_in_ptr_ptr @str_2

	str_2 print
