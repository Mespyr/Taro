struct String
  i64 size ^i8 data
struct Person
  String name i64 age

fun main()
	Person finn
	String name
	"Finn McGee\n" @name.data @name.size
	name @finn.name
	14 @finn.age

	&finn.name.size &finn.name.data 1 1 call3 pop
	&finn.age dump
