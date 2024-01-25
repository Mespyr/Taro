fun modulo(Int Int) [Int] / swp pop
fun puts(Int ^Char) 1 1 call3 pop

fun fizzbuzz(Int)
  Int i @i

  &i 15 modulo 0 = cjmpef divisible_15
  divisible_15:
    "FizzBuzz\n" puts
    jmpe else
  end

  &i 3 modulo 0 = cjmpef divisible_3
  divisible_3:
    "Fizz\n" puts
    jmpe else
  end

  &i 5 modulo 0 = cjmpef divisible_5
  divisible_5:
    "Buzz\n" puts
    jmpe else
  end

  else: &i dump end

fun main()
  1 loop:
    dup 100 > cjmpet loop
    dup fizzbuzz
    1 + jmp loop
  end pop
