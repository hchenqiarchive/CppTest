#include <type_traits>

struct Point {
	int x;
	int y;

	constexpr Point() = default;  // Warning C26495	Variable 'Point::x' is uninitialized.Always initialize a member variable(type.6).
	//constexpr Point() : x(), y() {}
	constexpr Point(int x, int y) : x(x), y(y) {}
};

static_assert(std::is_trivial_v<Point> == true);


int main() {
	Point x;  // uninitialized
	Point y = Point();  // zero initialized
	Point z(1, 2);
}