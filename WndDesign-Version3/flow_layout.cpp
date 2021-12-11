#pragma once

#include <vector>


using std::vector;

using uint = unsigned int;


struct Size {
	uint width;
	uint height;
};

struct Point {
	int x;
	int y;
};

struct Rect {
	Size size;
	Point point;
};


struct FlowLayout {
public:
	static uint UpdateHeight(uint width, vector<Rect>& elements, uint column_gap = 5, uint row_gap = 5) {
		if (elements.empty()) { 
			return 0; 
		}

		uint height = 0; 
		uint row_width = 0;
		uint row_height = 0; 

		for (auto& element : elements) {
			if (row_width > 0 && row_width + element.size.width > width) {
				height += row_height + row_gap;
				element.point = { 0, (int)height };
				row_width = element.size.width + column_gap;
				row_height = element.size.height;
			} else {
				element.point = { (int)row_width, (int)height };
				row_width += element.size.width + column_gap;
				row_height = std::max(row_height, element.size.height);
			}
		}

		return height + row_height;
	}

	static void Test() {
		vector<Rect> elements = { {10, 10}, {50, 10}, {20, 10}, {30, 10}, {20, 10} };
		uint height;
		height = FlowLayout::UpdateHeight(0, elements);
		height = FlowLayout::UpdateHeight(10, elements);
		height = FlowLayout::UpdateHeight(20, elements);
		height = FlowLayout::UpdateHeight(40, elements);
		height = FlowLayout::UpdateHeight(50, elements);
		height = FlowLayout::UpdateHeight(70, elements);
	}
};