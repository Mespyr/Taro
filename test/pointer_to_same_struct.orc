struct Node
  i64 value ^Node next

fun print_list_elems(^Node i64)
	i64 num_elements @num_elements
	i64 idx 0 @idx

	loop:
		&idx &num_elements = cjmpet loop

		dup &Node.value dump # print value of current node
		&Node.next # go to next node

		&idx 1 + @idx
		jmp loop
	end pop

fun main()
	Node n1 12 @n1.value
	Node n2 13 @n2.value

	# link nodes to each other to create a 'infinite' list
	n2 @n1.next
	n1 @n2.next

	n1 12 print_list_elems