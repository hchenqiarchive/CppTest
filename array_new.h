#pragma once

#include <memory>


struct Style {
	int s;

	Style() {
		s = 3;
	}

	//static void* operator new(size_t size) {
	//	return ::operator new(size);
	//}

	//static void* operator new[](size_t size) {
	//	return ::operator new[](size);
	//}	

	~Style() {

	}
};


struct TextBoxStyle : public Style {
	double t;

	TextBoxStyle() {
		t = 2.0;
	}
};


int main() {
	Style* style = new Style;
	TextBoxStyle* ts = new TextBoxStyle;

	Style* style2 = new Style[3];
	TextBoxStyle* ts2 = new TextBoxStyle[5];

	delete style;
	delete style2;
	delete[] ts2;
}