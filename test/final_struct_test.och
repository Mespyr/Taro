fun print_raw(Int ^Char)
  1 1 call3 pop

struct String
  Int size
  ^Char data

fun print(^String)
  String s @s
  &s.size &s.data print_raw 
  "\n" print_raw

struct Student
  String name
  String school
  Int grade
  Int age

fun print_student_info(^Student)
  Student s @s
  "Name: "      print_raw &s.name   print
  " - School: " print_raw &s.school print
  " - Grade: "  print_raw &s.grade  dump
  " - Age: "    print_raw &s.age    dump

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