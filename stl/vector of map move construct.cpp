#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

int main() {
	vector<map<int, int>> stack;
	stack.push_back({});
	stack.back().try_emplace(5, 2);
	int* ref = 0;
	for (auto& [index, var] : stack.back()) {
		ref = &var;
	}
	stack.emplace_back();
	// var: ???
}

// std::map(Or std::unordered_map) are NOT nothrow move constructable, so when the vector reallocates, the maps will be copied and destructed, so the reference invalidates.
// can use list instead of vector.