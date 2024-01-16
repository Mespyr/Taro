struct Rectangle(i64 width i64 height)

fun area(^Rectangle) [i64]
	Rectangle r @r
	&r.width &r.height *
end
fun perimeter(^Rectangle) [i64]
	Rectangle r @r
	&r.width 2 *
	&r.height 2 *
	+
end

fun main()
	Rectangle r
	12 @r.width
	13 @r.height
	
	r area dump
	r perimeter dump
end
