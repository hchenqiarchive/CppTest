#include <type_traits>
#include <memory>


struct Figure {

};

struct Rectangle : Figure {
	Rectangle(int w, int h) {}
};

using Point = struct { int x, y; };


struct FigureQueue {
	template<class FigureType, class ...Types, class = std::enable_if_t<std::is_base_of_v<Figure, FigureType>>>
	void append(Point point, Types&&... _Args) {
		std::make_unique<FigureType>(std::forward<Types>(_Args)...);
	}
};

int main() {
	FigureQueue fq;
	fq.append<Rectangle>(Point{ 0,0 }, 3, 2);
}