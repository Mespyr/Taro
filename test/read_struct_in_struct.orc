struct String
  Int size ^Char data
struct Row
  String row Int repeat

fun main()
	Row row1
	&row1.row 1 = dump # should put a type ^String on the stack