#include <math.h>
#include <stdint.h>
#include <stdio.h>

typedef float F32;
typedef struct {
    F32 x;
    F32 y;
} Vec2;

F32 distance(Vec2* p1, Vec2* p2) {
    return sqrt(pow(p2->x - p1->x, 2) + pow(p2->y - p1->y, 2));
}

void print(char* str) { printf(str); }

void puti(int64_t n) { printf("%ld\n", n); }
void putd(double n) { printf("%f\n", n); }
