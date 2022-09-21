# lcp
### The Lipsem Compiler Project

This is a remake of the [Lipsem programming language](https://github.com/Mespyr/lipsem) as a compiler instead of an interpreter.
Most of the lexer uses code from the [Ach programming language](https://github.com/Mespyr/ach).
*(both of these languages were created by me)*

At this point, the language is almost completely different from Lipsem other than the labels that you can jump to.
It is still very unreadable though.

### Improvements and Changes

- [x] LCP will compile programs down to assembly and use the `fasm` assembler to make an executable. This will make the programs many times faster than the interpreter.
- [x] LCP will have actual string pointers
- [x] labels will be allowed to jump to from anywhere within a function, making control flow easier
- [x] syscalls will be implemented, making the language be able to interact with the operating system
- [x] Basic stack manipulation
- [x] Function arguments and return values
- [x] Type Checking
- [ ] Datasets
	1. [ ] stage one:
	make memory block thingies like in Ach where you can define the size of the block like this:
	```
	memory EpicMemory 16 end
	```
	and access the parts of it using read and write commands


	2. [ ] stage two:
	make something like structs which will store data into sections kind of like this:
	```
	dataset EpicDataset
		num1 8 # 8 bytes
		num2 8
	end
	```

	and would be defined and accessed in the code like this:
	```
	EpicDataset epic_name_for_dataset

	# set num1 to 12
	12 @epic_name_for_dataset.num1
	# set num2 to 8
	8 @epic_name_for_dataset.num2

	# grab num1
	&epic_name_for_dataset.num1
	# grab num2
	&epic_name_for_dataset.num2

	+ dump
	```

	Datasets themselves cannot be modified on the stack, so to pass them as arguments for functions, you would write it like this:
	```
	dataset Point
		int x
		int y
	end

	fun calc_slope(Point:a Point:b)
		# dataset name + the name you want for the pointer
		@a.x @a.y
		@b.x @b.y
		# rest of code
	end

	fun main()
		Point a 5 @a.x 4 @a.y
		Point b 5 @b.x 0 @b.y

		# push pointers onto stack
		a b calc_slope
	end
	```


### Things I want to do

- [ ] make datasets a reality
- [ ] write code to generate a [prime spiral](https://mathimages.swarthmore.edu/index.php/Prime_spiral_(Ulam_spiral)) also known as a Ulam spiral.
