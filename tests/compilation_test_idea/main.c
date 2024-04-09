/* ########## boilerplate ########### */
typedef char              U8;
typedef long int          I64;
typedef int               I32;
typedef unsigned long int U64;
typedef unsigned int      U32;
typedef double            F64;
typedef float             F32;
typedef union {
    F32   _F32;
    F64   _F64;
    void* pass;
} FloatPasser;
void*  STACK[1024];
void** SP = STACK;

/* ########## types ############### */
typedef struct {
    F32 x;
    F32 y;
} Vec2;

/* ###### external ################ */
extern void puti(I64);
extern void putd(F64);
extern void print(U8*);
extern F32  distance(Vec2*, Vec2*);

/* ######### functions ############ */
void helloworld() {
    *SP = (void*)"Hello World!\n";
    SP++;  // PUSH String
    SP--;
    void* temp0 = *SP;     // pop to temp0
    print((U8*)temp0);  // call-external print(temp0)
}

void main_function_temp0() {  // push annonymous function
    SP--;
    void* temp0 = *SP;  // pop to temp0
    *SP = temp0;
    SP++;  // push temp0
    *SP = temp0;
    SP++;  // push temp0
    SP--;
    temp0 = *SP;  // pop to temp0
    SP--;
    void* temp1 = *SP;  // pop to temp1
    *SP = (void*)((I64)temp1 * (I64)temp0);
    SP++;  // temp0 * temp1
}

void main() {
    *SP = (void*)20;
    SP++;  // push I64 20
    SP--;
    void* temp0 = *SP;  // pop to temp0
    puti((I64)temp0);   // call-external PUTU(temp0)

    FloatPasser float_temp0;
    float_temp0._F64 = 12.3;  // set float_temp0 F64 to 12.3
    *SP = float_temp0.pass;
    SP++;  // push float_temp0.pass
    SP--;
    float_temp0.pass = *SP;  // pop to float_temp0.pass
    putd(float_temp0._F64);  // call-external PUTD(float_temp0._F64)

    *SP = (void*)helloworld;
    SP++;  // push function helloworld
    SP--;
    ((void (*)(void))(*SP))();  // call

    Vec2 Vec2_temp0 = {1.2, 2.7};
    *SP = (void*)&Vec2_temp0;
    SP++;  // new Vec2 as Vec2_temp0

    Vec2 Vec2_temp1 = {4.2, 6.9};
    *SP = (void*)&Vec2_temp1;
    SP++;  // new Vec2 as Vec2_temp1

    // bind (p1 p2)
    SP--;
    void* bind_p2 = *SP;  // pop to bind_p2
    SP--;
    void* bind_p1 = *SP;  // pop to bind_p1

    *SP = bind_p1;
    SP++;  // push bind_p1
    *SP = bind_p2;
    SP++;  // push bind_p2

    SP--;
    temp0 = *SP;  // pop to temp0
    SP--;
    void* temp1 = *SP;  // pop to temp1
    float_temp0._F32 = distance(
        (Vec2*)temp1,
        (Vec2*)
            temp0);  // call-external distance(temp1, temp0) to float_temp._F32
    *SP = float_temp0.pass;
    SP++;  // push float_temp.pass
    SP--;
    float_temp0.pass = *SP;  // pop to float_temp0.pass
    putd(float_temp0._F32);  // call-external PUTD(float_temp0._F32)

    *SP = (void*)12;
    SP++;  // push I64 12
    *SP = (void*)main_function_temp0;
    SP++;  // push main_function_temp0
    SP--;
    ((void (*)(void))(*SP))();  // call
    SP--;
    temp0 = *SP;       // pop to temp
    puti((I64)temp0);  // call-external PUTI(temp)
}
