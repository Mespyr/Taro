struct String
  i64 size
  ^i8 data
struct Person
  String name
  i64 age

fun main()
	Person person

	"Ben Dover" @person.name.data @person.name.size
	69 @person.age

	&person.name.size &person.name.data 1 1 call3 pop
	&person.age dump