/* ########## boilerplate ########### */
typedef char              U8;
typedef long int          I64;
typedef int               I32;
typedef unsigned long int U64;
typedef unsigned int      U32;
typedef double            F64;
typedef float             F32;
typedef _Bool             Bool;
typedef void*             Any;
#define func(type, name, ...) type (*name)(__VA_ARGS__)
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

I64 _mangle_what() {
	I64 _0 = 17;
	I64 _1 = 12;

	I64 _2 = _1;
	I64 _3 = 13;
	Bool _4 = _2 == _3;
	// values to pack after scope
	I64 _5;
	I64 _6;
	if (_4) {
	I64 _7 = _1;
	I64 _8 = _0;
	// pack
	_5 = _7;
	_6 = _8;
	} else {
	// pack
	_5 = _0;
	_6 = _1;
	}
	_5 = _5 - _6;
	return _5;
}

U64 _mangle_fib(U64 _0) {
	U64 _1 = _0;
	U64 _2 = 3;
	Bool _3 = _1 < _2;
	// values returned to function scope after if statement
	U64 _4;
	if (_3) {
	U64 _5 = 1;
	// pack scoped values back into outside scope
	_4 = _5;
	} else {
	U64 _6 = _0;
	U64 _7 = 1;
	_6 = _6 - _7;
	U64 _8 = _mangle_fib(_6);
	U64 _9 = 2;
	_0 = _0 - _9;
	U64 _10 = _mangle_fib(_0);
	_8 = _8 + _10;
	// pack scoped values back into outside scope
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

    Vec2 vec2_3;
	Vec2* _3 = &vec2_3;
    Vec2 vec2_4;
	Vec2* _4 = &vec2_4;
	_3->x = 1.2;
	_3->y = 2.7;
	_4->x = 4.2;
	_4->y = 6.9;

	F32 _5 = distance(*_3, *_4);
	putd(_5);

	I64 _6 = 12;
    func(I64, _7, I64) = _mangle_main_temp0;
	I64 _8  = _7(_6);
	puti(_8);

	U64 _9 = 40;
	U64 _10 = _mangle_fib(_9);
	puti(_10);

	I64 _11 = _mangle_what();
	puti(_11);
}

int main() {
	_mangle_main();
	return 0;
}
