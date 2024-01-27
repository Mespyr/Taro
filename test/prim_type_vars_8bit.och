struct String
  Int size
  ^Char data

fun extract_char(^String) [Char]
	String s @s           # set string to var on stack to access members
	Char char &s.data @char # read first char in ^i8 by abusing how @ works
	&char                 # read value in char

fun main()
	String str
	"Hello" @str.data @str.size
	str extract_char dump
