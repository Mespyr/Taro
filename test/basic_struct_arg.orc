struct String
  Int size
  ^Char data

fun ps(^String)
	String s @s
	&s.size dump

fun main()
	String s
	12 @s.size
	s ps
