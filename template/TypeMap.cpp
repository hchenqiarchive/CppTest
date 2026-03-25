#include <variant>
#include <memory>

template<typename Key, typename Value>
struct entry {
	using key = Key;
	using value = Value;
};

template<typename... Entries>
struct type_map {};


template<typename Map, typename Key>
struct get_mapped_type;

template<typename Key>
struct get_mapped_type<type_map<>, Key> {
	// No match → compile error
	static_assert(sizeof(Key) == 0, "Key not found in type_map");
};

template<typename Key, typename Value, typename... Rest>
struct get_mapped_type<type_map<entry<Key, Value>, Rest...>, Key> {
	using type = Value;
};

template<typename Head, typename... Rest, typename Key>
struct get_mapped_type<type_map<Head, Rest...>, Key> {
	using type = typename get_mapped_type<type_map<Rest...>, Key>::type;
};

template<typename Map, typename Key>
using get_mapped_type_t = typename get_mapped_type<Map, Key>::type;


struct Base {
	virtual ~Base() = default;
};

struct DerivedA : Base {};
struct DerivedB : Base {};

using MyMap = type_map<
	entry<int, DerivedA>,
	entry<double, DerivedB>
>;

using MyVariant = std::variant<int, double>;

std::unique_ptr<Base> make_object(const MyVariant& v) {
	return std::visit([](auto&& value) -> std::unique_ptr<Base> {
		using T = std::decay_t<decltype(value)>;
		using Mapped = get_mapped_type_t<MyMap, T>;
		return std::make_unique<Mapped>();
	}, v);
}

int main() {
	std::variant<int, double> v(1.0);
	auto o = make_object(v);
	return 0;
}

