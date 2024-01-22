struct String
  i64 size
  ^i8 data

fun ps(^String)
	String s @s
	&s.size dump

fun main()
	String s
	12 @s.size
	s ps
