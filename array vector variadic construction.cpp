#include <array>
#include <vector>
#include <memory>


inline constexpr size_t dynamic_extent = static_cast<size_t>(-1);


template<class T, size_t extent>
struct array_vector_selector {
	using type = std::array<T, extent>;

	template<class... Types>
	static std::enable_if_t<sizeof...(Types) == extent, std::array<T, extent>> construct(Types... args) {
		//return { T(std::forward<Types>(args))... };
		return { static_cast<T>(std::forward<Types>(args))... };
	}
};

template<class T>
struct array_vector_selector<T, dynamic_extent> {
	using type = std::vector<T>;

	template<class... Types>
	static std::vector<T> construct(Types... args) {
		//std::array<T, sizeof...(Types)> array{ T(std::forward<Types>(args))... };
		T array[] = { static_cast<T>(std::forward<Types>(args))... };
		return std::vector<T>(std::make_move_iterator(std::begin(array)), std::make_move_iterator(std::end(array)));
	}
};

template<class T, size_t extent = dynamic_extent>
using array_vector_selector_t = typename array_vector_selector<T, extent>::type;


template<class T, size_t extent = dynamic_extent>
struct A {
	array_vector_selector_t<T, extent> list;

	template<class... Types>
	A(Types... args) : list(array_vector_selector<T, extent>::construct(std::forward<Types>(args)...)) {}
};


int main() {
	//auto a = make_array<int>(3, 4, 5);
	//auto v = make_vector<int>(3, 4, 5);

	//auto a2 = make_array<std::unique_ptr<int>>(std::unique_ptr<int>(new int(3)), std::unique_ptr<int>(new int(4)));
	//auto v2 = make_vector<std::unique_ptr<int>>(std::unique_ptr<int>(new int(3)), std::unique_ptr<int>(new int(4)));

	A<int, 3> t0(1, 2, 3);
	A<int> t1(1, 2, 3);

	//char a = 0;
	//A<int*> m1(&a);  // error with static_cast

	A<std::unique_ptr<int>, 3> x0(std::unique_ptr<int>(new int(3)), std::unique_ptr<int>(new int(4)), std::unique_ptr<int>(new int(5)));
	A<std::unique_ptr<int>> x1(std::unique_ptr<int>(new int(3)), std::unique_ptr<int>(new int(4)));
}