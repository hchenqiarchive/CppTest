#include <future>
#include <iostream>

#include <Windows.h>


float f(int a) {
	Sleep(5000);
	std::cout << "f finished";
	return 3.0;
}

void g(float f) {
	// do something
}


int main() {
	auto fu = std::async(f, 3);
	std::cout << "main finished";
	Sleep(10000);
}


/*
Output:
0s: main finished
5s: f finished
10s: exited
*/