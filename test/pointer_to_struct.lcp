struct String
	i64 size
	^i8 data
end

fun String::init(i64 ^i8 ^String)
	# save the pointer to a variable
	# so I dont have to juggle the pointer on the stack
	# and to see if this could work
	^String s @s
	&s @String.data # get pointer from var
	&s @String.size
end

fun String::print(^String)
	dup &String.size
	swp &String.data
	1 1 call3 pop
end

fun main()
	String str
	"Hello World!\n" str String::init
	str String::print
end
