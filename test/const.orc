const HELLO 12 22 + end
const WORLD HELLO 35 + end
const MINUS WORLD HELLO - end
const MUL HELLO WORLD * end
const DIV WORLD HELLO / pop end
const MOD WORLD HELLO / swp pop end

fun main()
	HELLO dump
	WORLD dump
	MINUS dump
	MUL dump
	DIV dump
	MOD dump