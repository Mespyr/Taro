struct String
  Int size
  ^Char data

fun String::init(Int ^Char ^String)
	# save the pointer to a variable
	# so I dont have to juggle the pointer on the stack
	# and to see if this could work
	^String s @s
	&s @String.data # get pointer from var
	&s @String.size

fun String::print(^String)
	dup &String.size
	swp &String.data
	1 1 call3 pop

fun main()
	String str
	"Hello World!\n" str String::init
	str String::print
