#include <vector>
#include <iostream>
#include <span>

int main() {
	std::vector<int> vec; vec.reserve(10);
	if (int cnt = 10; true) { while (cnt--) { vec.push_back(cnt); } }
	std::span<int> vec_view(vec.data() + 2, 7);
	for (auto it = vec_view.begin(); it != vec_view.end(); ++it) { std::cout << *it; }
}