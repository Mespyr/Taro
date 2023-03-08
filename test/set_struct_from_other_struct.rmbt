struct String
	i64 size
	^i8 data
end

fun print(^String)
	String s @s # copy string on stack to s
	&s.size &s.data 1 1 call3 pop
end

fun main()
	String s
	"Hello World!\n" @s.data @s.size
	s print
end
