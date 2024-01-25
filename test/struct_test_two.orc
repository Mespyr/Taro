struct Rectangle
  Int width Int height

fun area(^Rectangle) [Int]
	Rectangle r @r
	&r.width &r.height *

fun perimeter(^Rectangle) [Int]
	Rectangle r @r
	&r.width 2 *
	&r.height 2 *
	+

fun main()
	Rectangle r
	12 @r.width
	13 @r.height
	
	r area dump
	r perimeter dump
