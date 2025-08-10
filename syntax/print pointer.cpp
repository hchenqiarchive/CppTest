#include <iostream>

int main() {
	const char* s = "hello";
	const int l[] = {1, 2};
	std::cout << s << std::endl;
	std::cout << (void*)s << std::endl;
	std::cout << l << std::endl;
}
