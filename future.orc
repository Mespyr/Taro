struct String
  i64 size
  ^i8 data
end

fun init |> String(i64 ^i8)
  @self.data
  @self.size
end

fun print |> String()
  &self.size &self.data
  1 1 call3 pop
end

fun test_pointer()
  ^String SPTR
  <String> @SPTR

  "Hello World!\n" &SPTR String.init
  &SPTR String.print

  &SPTR delete
end

fun main()
  String str
  "Hello World\n" str.init
  str.print
end
