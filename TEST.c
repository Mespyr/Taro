#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

typedef char    U8;
typedef int64_t I64;
typedef int32_t I32;

typedef union {
    float  F32;
    double F64;
    I64    pass;
} FloatPasser;
FloatPasser OCH_float;

/* custom types */

typedef struct {
    I64 size;
    U8* data;
} String;

/* stack definition */

void*    OCH_stack[1024];
uint32_t OCH_top = 0;

/* external */

void PUTU(int64_t n) {
    printf("%d", n);
    fflush(stdout);
}
void PUTD(double n) {
    printf("%f", n);
    fflush(stdout);
}

/* user defined */

void print() {  // I64 ^U8
    void *syscall_num, *arg0, *arg1, *arg2;
    OCH_stack[OCH_top++] = (void*)1;     // PUSH I64 1
    OCH_stack[OCH_top++] = (void*)1;     // PUSH I64 1
    syscall_num = OCH_stack[--OCH_top];  // POP to syscall_num
    arg0 = OCH_stack[--OCH_top];         // POP to arg0
    arg1 = OCH_stack[--OCH_top];         // POP to arg1
    arg2 = OCH_stack[--OCH_top];         // POP to arg2
                                         // SYSCALL syscall_num arg0 arg1 arg2
    OCH_stack[OCH_top++] = (void*)syscall((I64)syscall_num, arg0, arg1, arg2);
    OCH_top--;  // POP to nothing
}

void call_top_fn() {
    ((void (*)(void))(OCH_stack[--OCH_top]))(); // CALLFN
}

int main() {
    void* temp;

    OCH_stack[OCH_top++] = (void*)20;  // PUSH I64 20
    temp = OCH_stack[--OCH_top];       // POP to temp
    PUTU((I64)temp);                   // CALL_EXTERNAL PUTU

    OCH_stack[OCH_top++] = (void*)1;     // PUSH I64 1
    OCH_stack[OCH_top++] = (void*)"\n";  // PUSH ^U8
    print();                             // CALL print

    OCH_float.F64 = 12.3;  // PUSH F64 12.3
    OCH_stack[OCH_top++] = (void*)OCH_float.pass;
    OCH_float.pass = (I64)OCH_stack[--OCH_top];  // POP to global float pass
    PUTD(OCH_float.F64); // CALL_EXTERNAL PUTD

    OCH_stack[OCH_top++] = (void*)1;     // PUSH I64 1
    OCH_stack[OCH_top++] = (void*)"\n";  // PUSH ^U8
    print();                             // CALL print

    OCH_stack[OCH_top++] = malloc(sizeof(String));  // PUSH new(String)
    free(OCH_stack[--OCH_top]);                     // delete String

    OCH_stack[OCH_top++] = (void*)14;                // PUSH I64 14
    OCH_stack[OCH_top++] = (void*)"Hello World!\n";  // PUSH ^U8
    OCH_stack[OCH_top++] = (void*)print; // PUSHFN print
    OCH_stack[OCH_top++] = (void*)call_top_fn; // PUSHFN print
    OCH_stack[OCH_top++] = (void*)call_top_fn; // PUSHFN print
    OCH_stack[OCH_top++] = (void*)call_top_fn; // PUSHFN print
    OCH_stack[OCH_top++] = (void*)call_top_fn; // PUSHFN print
    OCH_stack[OCH_top++] = (void*)call_top_fn; // PUSHFN print
    ((void (*)(void))(OCH_stack[--OCH_top]))(); // CALLFN

    return 0;
}
