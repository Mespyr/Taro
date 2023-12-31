fun fib(i64) [i64]
	i64 n @n
	i64 i 0 @i

	0 1 loop:
		&n &i > cjmpef loop
		swp over +
		&i 1 + @i
		jmp loop
	end pop
end

fun main()
	100000000 fib dump
end
