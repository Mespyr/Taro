struct String
  i64 size ^i8 data

fun new_String(i64 ^i8) [^String]
	<String>
	swp over @String.data
	swp over @String.size

fun print(^String)
	dup &String.size
	swp &String.data
	1 1 call3 pop

fun main()
	"Hello World\n" new_String
	dup print
	delete
