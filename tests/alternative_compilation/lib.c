#include <math.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    float x;
    float y;
} Vec2;

float distance(Vec2 p1, Vec2 p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

void print(char* str) { printf(str); }

void puti(int64_t n) { printf("%ld\n", n); }
void putd(double n) { printf("%f\n", n); }
