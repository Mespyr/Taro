struct String(i64 size ^i8 data)
struct Row(String row i64 repeat)

fun main()
	Row row1
	&row1.row 1 = dump # should put a type ^String on the stack
end
