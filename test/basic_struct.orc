# experiment with diffeent structs
struct String
  Int size
  ^Char data

struct TwoCount
  Int c1
  Int c2

struct TwoChars
  Char c1
  Char c2

fun main()
	String name
	"Hello World!\n" @name.data @name.size
	&name.size &name.data 1 1 call3 pop

	TwoCount two_count
	0 @two_count.c1
	0 @two_count.c2

	&two_count.c1 1 + @two_count.c1
	&two_count.c2 10 + @two_count.c2

	&two_count.c1 dump
	&two_count.c2 dump

	TwoChars 2c
	69 @2c.c1
	65 @2c.c2
	&2c.c1 dump
	&2c.c2 dump