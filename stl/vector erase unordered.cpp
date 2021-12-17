#include <vector>

template<class T>
inline void unordered_erase(std::vector<T>& vector, size_t index) {
	if (index >= vector.size()) { throw; }
	if (index < vector.size() - 1) {
		std::iter_swap(vector.begin() + index, vector.end() - 1);
	}
	vector.pop_back();
}
