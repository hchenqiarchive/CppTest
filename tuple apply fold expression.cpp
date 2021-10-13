#include <tuple>
#include <iostream>

void f(auto arg) {
	std::cout << arg;
}

int main() {
	auto tp = std::make_tuple(1, 3.0, "abc");
	std::apply([](auto... args) { ((std::cout << args), ...); }, tp);
	std::apply([](auto... args) { (f(args), ...); }, tp);
}