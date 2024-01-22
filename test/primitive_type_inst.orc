fun zero_init(^i64) [^i64]
	dup 0 swp @i64

fun main()
	<i64> zero_init
	dup &i64 1 + over @i64
	&i64 dump
