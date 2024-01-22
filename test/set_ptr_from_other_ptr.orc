fun crazy_copy(^i64 ^i64)
	^i64 b @b
	^i64 a @a
	&a dup &i64 420 + swp @i64
	&a &b @i64

fun main()
	i64 a 69 @a
	i64 b
	a b crazy_copy
	
	"a = " 1 1 call3 pop
	&a dump

	"b = " 1 1 call3 pop
	&b dump