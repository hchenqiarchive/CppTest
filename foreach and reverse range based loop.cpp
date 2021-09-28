/*
References:
	https://stackoverflow.com/questions/8542591/c11-reverse-range-based-for-loop
	https://stackoverflow.com/questions/46703360/what-is-the-proper-way-of-creating-a-thin-struct-wrapper-for-an-object
*/

#include <vector>
#include <iostream>
#include <algorithm>

// -------------------------------------------------------------------
// --- Reversed iterable

template <typename T>
struct reversion_wrapper { T& iterable; };

template <typename T>
auto begin(reversion_wrapper<T> w) { return std::rbegin(w.iterable); }

template <typename T>
auto end(reversion_wrapper<T> w) { return std::rend(w.iterable); }

template <typename T>
reversion_wrapper<T> reverse(T& iterable) { return { iterable }; }

//template <typename T>
//reversion_wrapper<T> reverse(T&& iterable) { return { iterable }; }

int main() {
	// Fine.
	for (auto it : std::vector<int>{1, 2, 3, 4}) {
		std::cout << it << std::endl;
	}

	// Wrong! Vector destructs before loop started!
	for (auto it : reverse(std::vector<int>{ 1, 2, 3, 4 })) {
		std::cout << it << std::endl;
	}

	// Fine.
	std::vector<int> v{ 1, 2, 3, 4 };
	for (auto it : reverse(v)) {
		std::cout << it << std::endl;
	}

	// Use for each:
	std::for_each(std::begin(v), std::end(v), [](auto i) {std::cout << i << std::endl; });
	for_each(std::rbegin(v), std::rend(v), [](auto i) {std::cout << i << std::endl; });  
	// Wait... Why it works without std:: prefix ??? (Argument Dependent Lookup)

}