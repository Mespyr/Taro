struct Rectangle(i64 width i64 height)

fun print(i64 ^i8)
	1 1 call3 pop
end

fun scale(i64 ^Rectangle)
	^Rectangle r @r
	i64 deg @deg

	&r &Rectangle.width &deg *
	&r @Rectangle.width

	&r &Rectangle.height &deg *
	&r @Rectangle.height
end

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
end

fun area(^Rectangle) [i64]
	dup &Rectangle.width swp &Rectangle.height *
end

fun main()
	Rectangle rect
	5 @rect.width
	6 @rect.height
	rect display
	rect area dump

	2 rect scale
	rect display
	rect area dump
end
