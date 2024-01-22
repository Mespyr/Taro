# print numbers 9-0
# when reaching 6, print 69 instead

fun main()
	10 loop:
		1 -
		
		dup 6 = cjmpf iflabel
			69 dump
			jmpe iflabel
		iflabel:
			dup dump
		end

		dup cjmpt loop
	end