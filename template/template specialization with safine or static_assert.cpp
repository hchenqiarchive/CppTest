#include <type_traits>

struct Vertical {};
struct Horizontal {};

/*
template<class Direction, class = std::enable_if_t<std::is_same_v<Direction, Horizontal> || std::is_same_v<Direction, Vertical>>>
struct is_vertical {
	constexpr static bool value = false;
};

template<>
struct is_vertical<Vertical> {
	constexpr static bool value = true;
};
*/

/*
template<class>
struct is_vertical {
	static_assert(false, "invalid template parameter");
};

template<>
struct is_vertical<Vertical> {
	constexpr static bool value = true;
};

template<>
struct is_vertical<Horizontal> {
	constexpr static bool value = false;
};
*/

template<class>
constexpr bool is_vertical;

template<>
constexpr bool is_vertical<Vertical> = true;

template<>
constexpr bool is_vertical<Horizontal> = false;


int main() {
	bool b;
	//b = is_vertical<Horizontal>::value;
	//b = is_vertical<Vertical>::value;
	//b = is_vertical<int>;

	b = is_vertical<Vertical>;
	b = is_vertical<Horizontal>;
}