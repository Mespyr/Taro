fun crazy_copy(^Int ^Int)
	^Int b @b
	^Int a @a
	&a dup &Int 420 + swp @Int
	&a &b @Int

fun main()
	Int a 69 @a
	Int b
	a b crazy_copy
	
	"a = " 1 1 call3 pop
	&a dump

	"b = " 1 1 call3 pop
	&b dump