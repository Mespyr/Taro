struct String
  i64 size ^i8 data

fun print(^String)
	dup &String.size swp
	&String.data 1 1 call3 pop

fun main()
	<String> # create an instance of the String struct, without assigning a variable name
	^String pS @pS
	"Hello World!\n"
	&pS @String.data
	&pS @String.size
	&pS print