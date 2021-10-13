#include <variant>
#include <iostream>
#include <string>
#include <vector>


struct A {
	std::string val;
};

struct B {
	std::vector<int> val;
};


/*
* Reference:
* https://stackoverflow.com/questions/60564132/default-constructing-an-stdvariant-from-index/60567091#60567091
*/

//template <class Variant, std::size_t I>
//Variant _construct_variant_helper(std::size_t index) {
//	if constexpr (I < std::variant_size_v<Variant>) {
//		return index == I ? Variant(std::in_place_index<I>) : _construct_variant_helper<Variant, I + 1>(index);
//	}
//	throw std::runtime_error("invalid variant index");
//}
//
//template <class Variant>
//Variant construct_variant(std::size_t index) {
//	return _construct_variant_helper<Variant, 0>(index);
//}


template<size_t I, class... Types>
std::variant<Types...> construct_variant(std::size_t index) {
	if constexpr (I < sizeof...(Types)) {
		return index == I ? std::variant<Types...>(std::in_place_index<I>) : construct_variant<I + 1, Types...>(index);
	}
	throw std::invalid_argument("invalid variant index");
}

template<class... Types>
std::variant<Types...> construct_variant(std::size_t index) {
	return construct_variant<0, Types...>(index);
}


int main() {
	int index; std::cin >> index;
	auto v = construct_variant<A, B>(index);
}