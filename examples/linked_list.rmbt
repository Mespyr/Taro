struct Node
  i64 data
  ^Node next
end

struct LinkedList
  Node head
  i64 size
end

fun LinkedList::init(^LinkedList)
  0 swp @LinkedList.size
end

fun LinkedList::append(i64 ^LinkedList)
  ^LinkedList LL @LL
  i64 value @value
  &LL &LinkedList.size 0 = cjmpf append 

  &value &LL @LinkedList.head.data
  jmpe append

  append:
    <Node> dup &value swp @Node.data
    &LL &LinkedList.head
    0 loop:
      dup &LL &LinkedList.size 1 - = cjmpet loop
      swp &Node.next
      swp 1 +
      jmp loop
    end pop
    @Node.next
  end

  &LL &LinkedList.size 1 + &LL @LinkedList.size
end

fun LinkedList::print(^LinkedList)
  LinkedList LL @LL
  &LL.head 0 loop:
    dup &LL.size = cjmpet loop
    swp dup &Node.data dump
    &Node.next # go to next node
    swp 1 +
    jmp loop
  end pop pop
end

fun LinkedList::pop(^LinkedList)
  ^LinkedList LL @LL
  &LL &LinkedList.head
  0 loop:
    dup &LL &LinkedList.size 1 - = cjmpet loop
    swp &Node.next
    swp 1 +
    jmp loop
  end pop
  delete
  &LL &LinkedList.size 1 - &LL @LinkedList.size
end

fun LinkedList::clear(^LinkedList)
  ^LinkedList LL @LL
  &LL &LinkedList.head &Node.next

  0 loop:
    dup &LL &LinkedList.size 1 - = cjmpet loop
    swp dup &Node.next
	swp delete
	swp 1 +
	jmp loop
  end pop pop
  0 &LL @LinkedList.size
end

fun print(i64 ^i8)
  1 1 call3 pop
end

fun main()
  LinkedList LL
  LL LinkedList::init

  12 LL LinkedList::append
  13 LL LinkedList::append
  14 LL LinkedList::append
  15 LL LinkedList::append
  16 LL LinkedList::append

  "First Linked List Iteration:\n" print
  LL LinkedList::print

  LL LinkedList::pop
  LL LinkedList::pop

  "Linked List Iteration After Popping:\n" print
  LL LinkedList::print

  LL LinkedList::clear

  "Size of Linked List After Clearing\n" print
  &LL.size dump
end
