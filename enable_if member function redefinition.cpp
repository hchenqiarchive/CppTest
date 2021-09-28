#include <type_traits>


constexpr size_t dynamic_extent = -1;


template<size_t size = dynamic_extent>
struct A {
	template<class = std::enable_if_t<size == dynamic_extent>>
	A(int a) {}

	// ! redefinition
	//template<class = std::enable_if_t<size != dynamic_extent>>
	//A(int a) {}

	template<class = std::enable_if_t<size != dynamic_extent>>
	A(double b) {}

	template<class = std::enable_if_t<size == dynamic_extent>>
	void f() {}

	// ! redefinition
	//template<class = std::enable_if_t<size != dynamic_extent>>
	//void f() {}

	//template<class = void>
	//std::void_t<std::enable_if_t<size != dynamic_extent>> f() {}

	template<class = std::enable_if_t<size != dynamic_extent>>
	void g() {}
};


int main() {
	A a(1);
	a.f();
}