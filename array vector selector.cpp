#include <array>
#include <vector>


constexpr size_t dynamic_extent = -1;


template<class T, size_t size>
struct ArrayBase {
	using type = std::array<T, size>;
};

template<class T>
struct ArrayBase<T, dynamic_extent> {
	using type = std::vector<T>;
};


template<class T, size_t size = dynamic_extent>
using Array = ArrayBase<T, size>::type;


int main() {
	Array<int> a;
	Array<int, 5> b;
}