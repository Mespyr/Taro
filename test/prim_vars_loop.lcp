fun main()
	i64 count
	10 @count # initialize

	# count down
	loop:
		&count dump
		&count 0 = cjmpet loop

		&count 1 - @count
		jmp loop
	end

	"Done!\n" 1 1 call3 pop
end
