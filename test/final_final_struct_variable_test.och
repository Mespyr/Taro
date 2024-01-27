struct Rectangle
  Int width
  Int height

fun print(Int ^Char)
	1 1 call3 pop

fun scale(Int ^Rectangle)
	^Rectangle r @r
	Int deg @deg

	&r &Rectangle.width &deg *
	&r @Rectangle.width

	&r &Rectangle.height &deg *
	&r @Rectangle.height

fun display(^Rectangle)
	Rectangle r @r
	0 height_loop:
		# if not less than the height
		dup &r.height < cjmpef height_loop

		0 row_loop:
			dup &r.width <= cjmpef row_loop
			"+" print
			1 + jmp row_loop
		end pop
		"\n" print
	
		1 + jmp height_loop
	end pop

fun area(^Rectangle) [Int]
	dup &Rectangle.width swp &Rectangle.height *

fun main()
	Rectangle rect
	5 @rect.width
	6 @rect.height
	rect display
	rect area dump

	2 rect scale
	rect display
	rect area dump
