fun print_raw(i64 ^i8)
	1 1 call3 pop
end

struct String(i64 size ^i8 data)

fun print(^String)
	String s @s
	&s.size &s.data print_raw 
	"\n" print_raw
end

struct Student (
  String name
  String school
  i64 grade
  i64 age
)

fun print_student_info(^Student)
	Student s @s
	"Name: "      print_raw &s.name   print
	" - School: " print_raw &s.school print
	" - Grade: "  print_raw &s.grade  dump
	" - Age: "    print_raw &s.age    dump
end

fun main()
	Student finn
	"Finn McGee" @finn.name.data @finn.name.size
	"Example School" @finn.school.data @finn.school.size
	9  @finn.grade
	15 @finn.age
	finn print_student_info

	Student ben
	"Ben Dover" @ben.name.data @ben.name.size
	"Example School" @ben.school.data @ben.school.size
	10 @ben.grade
	16 @ben.age
	ben print_student_info
end
