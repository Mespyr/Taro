#include <stdbool.h>

/* ########## boilerplate ########### */
typedef char              U8;
typedef long int          I64;
typedef int               I32;
typedef unsigned long int U64;
typedef unsigned int      U32;
typedef double            F64;
typedef float             F32;
typedef bool Bool;
typedef void* Any;
#define func(type, name, ...) type (*name)(__VA_ARGS__)
typedef union {
    F32 _F32;
    F64 _F64;
    Any pass;
} FloatPass;
FloatPass fp;
Any f32_to_void(F32 n) {
	fp._F32 = n;
	return fp.pass;
}
Any f64_to_void(F64 n) {
	fp._F64 = n;
	return fp.pass;
}
/* ########## types ############### */
typedef struct {
    F32 x;
    F32 y;
} Vec2;

/* ###### external ################ */
extern void puti(I64);
extern void putd(F64);
extern void print(U8*);
extern F32  distance(Vec2, Vec2);

/* ######### functions ############ */
void _mangle_helloworld() {
    U8* _0 = "Hello World!\n";
    print(_0);
}

U64 _mangle_fib(U64 _0) {
	U64 _1 = _0;
	U64 _2 = 3;
	Bool _3 = _1 < _2;
	U64 _4;
	if (_3) {
		U64 _5 = 1;
		_4 = _5;
	}
	else {
		U64 _6 = _0;
		U64 _7 = 1;
		_6 = _6 - _7;
		U64 _8 = _mangle_fib(_6);
		U64 _9 = 2;
		_0 = _0 - _9;
		U64 _10 = _mangle_fib(_0);
		_8 = _8 + _10;
		_4 = _8;
	}
	return _4;
}

I64 _mangle_main_temp0(I64 _0) {
	I64 _1 = _0;
    _0 = _0 * _1;
	return _0;
}

void _mangle_main() {
    I64 _0 = 20;
    puti(_0);
	F64 _1 = 12.3;
    putd(_1);

    func(void, _2) = _mangle_helloworld;
    _2();
	_mangle_helloworld();

    Vec2 _3;
    Vec2 _4;
	_3.x = 1.2;
	_3.y = 2.7;
	_4.x = 4.2;
	_4.y = 6.9;

	F32 _5 = distance(_3, _4);
	putd(_5);

	I64 _6 = 12;
    func(I64, _7, I64) = _mangle_main_temp0;

	I64 _8  = _7(_6);
	puti(_8);

	U64 _9 = 30;
	U64 _10 = _mangle_fib(_9);
	puti(_10);
}

int main() {
	_mangle_main();
	return 0;
}
