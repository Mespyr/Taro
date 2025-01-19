#include <memory>
#include <cstdint>
#include <string>
#include <iostream>

#define ptr(n) std::shared_ptr<n>

void puts(ptr(std::string) str) {
	std::cout << *str;
}

void puti(int64_t n) {
	std::cout << n;
}

void putf(double n) {
	std::cout << n;
}

struct vec2 {
	double x;
	double y;
};
double distance(ptr(vec2) p1, ptr(vec2) p2) {
	return (p2->y - p1->y) / (p2->x - p1->x);
}
