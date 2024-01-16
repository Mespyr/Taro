# all of the following names should cause an error

struct Epic.Cap(i64 h)
struct E@Sports(i64 h)
struct HEH&qkw(i64 h)

# except this
struct TwoIs(i64 i1 i64 i2)

fun main()
	TwoIs e.a.Sports
	TwoIs @eaSports
	TwoIs &eaSports
end
