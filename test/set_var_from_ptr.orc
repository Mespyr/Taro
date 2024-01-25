fun dplus(^Int) [^Int]
	Int n @n
	# n * 2 + n
	&n 2 * &n + @n
	n

fun main()
	Int count
	10 @count
	count dplus @count
	&count dump
