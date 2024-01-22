# check use of ^ symbol in arg stack and ret stack
# as well as lack of it

fun get_cool_str() [i64 ^i8]
	"Hello World!\n"

fun print(i64 ^i8)
	1 1 call3 pop

fun main()
	get_cool_str print
