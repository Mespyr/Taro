/* ########## boilerplate ########### */
#define NULL ((void*)0)
typedef char              U8;
typedef long int          I64;
typedef int               I32;
typedef unsigned long int U64;
typedef unsigned int      U32;
typedef double            F64;
typedef float             F32;
typedef void* Any;
typedef void (*Func)(Any*);
typedef union {
    F32 _F32;
    F64 _F64;
    Any pass;
} FloatPass;
FloatPass fp;
Any f32_pass(F32 n) {
	fp._F32 = n;
	return fp.pass;
}
Any f64_pass(F64 n) {
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
void _mangle_helloworld(Any* stack) {
    U8* _0 = "Hello World!\n";
    print(_0);  // call-external print(_0)
}

void _mangle_main_temp0(Any* stack) {
	// unpack stack
    I64 _0 = (I64) stack[0];

	I64 _1 = _0;
    _0 = _0 * _1;

	// pack stack
	stack[0] = (Any)_0;
}

void _mangle_main(Any* stack) {
    I64 _0 = 20;
    puti(_0);
	F64 _1 = 12.3;
    putd(_1);

    Func _2 = _mangle_helloworld;
    _2(NULL);

	_mangle_helloworld(NULL);

    Vec2 _3;
    Vec2 _4;
	_3.x = 1.2;
	_3.y = 2.7;
	_4.x = 4.2;
	_4.y = 6.9;

	F32 _5 = distance(_3, _4);
	putd(_5);

	I64 _6 = 12;
    Func _7 = _mangle_main_temp0;

	// construct stack
	Any _stk_temp0[1];
	_stk_temp0[0] = (Any)_6;
	// call function
	_7(_stk_temp0);
	// unpack
	I64 _8 = (I64) _stk_temp0[0];

	puti(_8);
}

int main() {
	_mangle_main(NULL);
	return 0;
}
