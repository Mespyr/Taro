struct String(i64 size ^i8 data)

fun extract_char(^String) [i8]
	String s @s           # set string to var on stack to access members
	i8 char &s.data @char # read first char in ^i8 by abusing how @ works
	&char                 # read value in char
end

fun main()
	String str
	"Hello" @str.data @str.size
	str extract_char dump
end
