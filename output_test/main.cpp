// import some required standard libs
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstdint>

// some macros to reduce c++ output size
#define ptr(n) std::shared_ptr<n>
#define make(n, ...) std::make_shared<n>(__VA_ARGS__)

// define builtin types
typedef int64_t i64;
typedef int32_t i32;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef float f32;
typedef double f64;
typedef std::string string;
#define list(n) std::vector<n>

// use-defined types
struct vec2 {
	f64 x;
	f64 y;
};

// external functions
extern void puts(ptr(string));
extern void puti(i64);
extern void putf(f64);
extern f64 distance(ptr(vec2), ptr(vec2));

// user-defined functions
void _mangle_dotnl() {
	auto _0 = make(string, "\n");
	puts(_0);
}

void _mangle_putln(ptr(string) _0) {
	puts(_0);
	_mangle_dotnl();
}

void _mangle_main() {
	auto _0 = make(string, "Hello World!");
	_mangle_putln(_0);
	i64 _1 = 12;
	i64 _2 = 13;
	i64 _3 = _1 + _2;
	puti(_3);
	_mangle_dotnl();
	f64 _4 = 1.2;
	f64 _5 = 3.8;
	auto _6 = make(vec2);
	_6->x = _4;
	_6->y = _5;
	f64 _7 = 2.6;
	f64 _8 = 3.9;
	auto _9 = make(vec2);
	_9->x = _7;
	_9->y = _8;
	f64 _10 = distance(_6, _9);
	putf(_10);
	_mangle_dotnl();
}

// entry point
int main() {
	_mangle_main();
	return 0;
}
