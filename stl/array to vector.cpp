#include <array>
#include <vector>
#include <memory>



template<class T, size_t size>
std::vector<T> array_to_vector(std::array<T, size>&& a) {
	return std::vector<T>(std::make_move_iterator(std::begin(a)), std::make_move_iterator(std::end(a)));
}

template<class T, size_t size>
std::vector<T> array_to_vector(T (&&a)[size]) {
	return std::vector<T>(std::make_move_iterator(std::begin(a)), std::make_move_iterator(std::end(a)));
}


int main() {
	std::unique_ptr<int> a[] = {
		std::unique_ptr<int>(new int(3)),
		std::unique_ptr<int>(new int(4)),
		std::unique_ptr<int>(new int(5))
	};

	std::array<std::unique_ptr<int>, 3> a2 = {
		std::unique_ptr<int>(new int(3)),
		std::unique_ptr<int>(new int(4)),
		std::unique_ptr<int>(new int(5))
	};

	//std::begin(a);

	// error
	// move only initializer list can not be used to initialize std::vector
	//std::vector<std::unique_ptr<int>> v = {
	//	std::unique_ptr<int>(new int(3)),
	//	std::unique_ptr<int>(new int(4)),
	//	std::unique_ptr<int>(new int(5))
	//};

	std::vector<std::unique_ptr<int>> v = array_to_vector(std::move(a));
	std::vector<std::unique_ptr<int>> v2 = array_to_vector(std::move(a2));

	std::vector<std::unique_ptr<int>> v3 = array_to_vector(std::array<std::unique_ptr<int>, 3>{
		std::unique_ptr<int>(new int(3)),
		std::unique_ptr<int>(new int(4)),
		std::unique_ptr<int>(new int(5))
	});
}