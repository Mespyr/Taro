fun linear_fib(Int) [Int]
  Int n @n
  Int i 1 @i

  0 1 loop:
    &n &i > cjmpef loop
	swp over +
    &i 1 + @i
    jmp loop
  end swp pop

fun recursive_fib(Int) [Int]
  Int n @n
  Int res

  &n &n 0 =
  &n 1 =
  or cjmpet recursion

  recursion:
    1 - recursive_fib
    &n 2 - recursive_fib +
  end

fun main()
  1000000 linear_fib dump
  40 recursive_fib dump
