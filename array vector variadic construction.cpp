
#include <array>
#include <vector>
#include <memory>


using uint = unsigned int;



template<class T, class... Types>
std::array<T, sizeof...(Types)> make_array(Types... args) { return { T(std::forward<Types>(args))... }; }

template<class T, class... Types>
std::vector<T> make_vector(Types... args) {
	auto a = make_array<T>(std::forward<Types>(args)...);
	return std::vector<T>(std::make_move_iterator(std::begin(a)), std::make_move_iterator(std::end(a)));
}


constexpr uint dynamic_extent = -1;


template<class T, size_t size>
struct array_vector_selector {
	using type = std::array<T, size>;

	template<class T, class... Types>
	static std::array<T, size> construct(Types... args) { return { T(std::forward<Types>(args))... }; }
};

template<class T>
struct array_vector_selector<T, dynamic_extent> {
	using type = std::vector<T>;

	template<class T, class... Types>
	static std::vector<T> construct(Types... args) { return make_vector<T>(std::forward<Types>(args)...); }
};

template<class T, size_t size = dynamic_extent>
using array_vector_selector_t = typename array_vector_selector<T, size>::type;


template<class T, size_t size = dynamic_extent>
struct A {
	array_vector_selector_t<T, size> list;

	template<class... Types>
	A(Types... args) : list(array_vector_selector<T, size>::template construct<T>(std::forward<Types>(args)...)) {}
};


int main() {
	auto a = make_array<int>(3, 4, 5);
	auto v = make_vector<int>(3, 4, 5);

	auto a2 = make_array<std::unique_ptr<int>>(std::unique_ptr<int>(new int(3)), std::unique_ptr<int>(new int(4)));
	auto v2 = make_vector<std::unique_ptr<int>>(std::unique_ptr<int>(new int(3)), std::unique_ptr<int>(new int(4)));

	A<int, 3> t0(1, 2, 3);
	A<int> t1(1, 2, 3);

	A<std::unique_ptr<int>, 3> x0(std::unique_ptr<int>(new int(3)), std::unique_ptr<int>(new int(4)));
	A<std::unique_ptr<int>> x1(std::unique_ptr<int>(new int(3)), std::unique_ptr<int>(new int(4)));
}