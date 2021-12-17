#include <type_traits>


template<class T, class = void>
constexpr size_t block_size = -1;

template<class T>
constexpr size_t block_size<T, std::enable_if_t<std::is_arithmetic_v<T>>> = sizeof(T);

struct A {};

template<>
constexpr size_t block_size<A> = 10;

struct B {};


int main() {
	block_size<int>;  // 4
	block_size<char>;  // 1
	block_size<A>;  // 10
	block_size<B>;  // -1
}