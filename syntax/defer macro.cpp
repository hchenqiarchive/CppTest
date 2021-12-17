#pragma once

#include <functional>


struct _do_defer {
    std::function<void()> f;
    _do_defer(std::function<void()> f) : f(f) {}
    ~_do_defer() { f(); }
};


#define _DEFER_1(x, y) x##y
#define _DEFER_2(x, y) _DEFER_1(x, y)
#define _DEFER_0(x)    _DEFER_2(x, __LINE__)
#define defer(func)   _do_defer _DEFER_0(_defered_option)(func)


/* 
// defer test
#include "defer.h"
#include <iostream>
using namespace std;
int main() {
	int i;
	i = 1;
	defer([&]() { cout << i << endl; });
	i = 2;
	defer([=]() { cout << i << endl; });
	i = 3;
	defer([=]() { cout << i << endl; });
}
// Output:
// 3
// 2
// 3
*/