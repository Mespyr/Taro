/* ########## boilerplate ########### */
typedef char              U8;
typedef long int          I64;
typedef int               I32;
typedef unsigned long int U64;
typedef unsigned int      U32;
typedef double            F64;
typedef float             F32;
typedef U8* String;
typedef union {
    F32 _F32;
    F64 _F64;
    void* pass;
} FloatPasser;
void* OCH_stack[1024];
U32 OCH_top = 0;

/* ########## types ############### */
typedef struct {
    F32 x;
	F32 y;
} Vec2;

/* ###### external ################ */
extern void PUTI(I64);
extern void PUTD(F64);
extern void print(String);
extern F32 distance(Vec2*, Vec2*);

/* ######### functions ############ */
void helloworld() {
    OCH_stack[OCH_top++] = (void*)"Hello World!\n";  // PUSH String
	void *temp = OCH_stack[--OCH_top];
	print((String)temp);
}

int main() {
    OCH_stack[OCH_top++] = (void*)20;  // PUSH I64 20
    void *temp = OCH_stack[--OCH_top];       // POP to temp
    PUTI((I64)temp);                   // CALL_EXTERNAL PUTI

    FloatPasser float_temp;  // PUSH F64 12.3
	float_temp._F64 = 12.3;
    OCH_stack[OCH_top++] = float_temp.pass;
    float_temp.pass = OCH_stack[--OCH_top];  // POP to float temp
    PUTD(float_temp._F64); // CALL_EXTERNAL PUTD

    OCH_stack[OCH_top++] = (void*)helloworld; // PUSHFN print
    ((void (*)(void))(OCH_stack[--OCH_top]))(); // CALLFN

	// new Vec2
	Vec2 Vec2_temp = {1.2, 2.7};
    OCH_stack[OCH_top++] = (void*)&Vec2_temp;
	// new Vec2
	Vec2 Vec2_temp1 = {4.2, 6.9};
    OCH_stack[OCH_top++] = (void*)&Vec2_temp1;
	// bind p1 p2
    void *bind_p2 = OCH_stack[--OCH_top];
    void *bind_p1 = OCH_stack[--OCH_top];

    OCH_stack[OCH_top++] = bind_p1; // PUSH p1
    OCH_stack[OCH_top++] = bind_p2; // PUSH p2

    temp = OCH_stack[--OCH_top]; // POP to temp
    void *temp1 = OCH_stack[--OCH_top]; // POP to temp1
	float_temp._F32 = distance((Vec2*)temp1, (Vec2*)temp); // CALL-EXTERNAL distance
    OCH_stack[OCH_top++] = float_temp.pass; // PUSH result
	float_temp.pass = OCH_stack[--OCH_top];  // POP to global float pass
    PUTD(float_temp._F32); // CALL_EXTERNAL PUTD

    return 0;
}
