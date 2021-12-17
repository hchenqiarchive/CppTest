#include <iostream>
#include <type_traits>


using namespace std;


class WndObject {
public:
	WndObject(int para) {}
protected:
	virtual void OnDraw() {
		cout << "WndObject Draw " << endl;
	}
	virtual void OnSize() {
		cout << "WndObject Size " << endl;
	}
};


template<class Wnd>
class Background : public Wnd {
public:
	using Wnd::Wnd;
protected:
	int background;
protected:
	virtual void OnDraw() override {
		cout << "Background Draw " << background << endl;
		Wnd::OnDraw();
	}
};


template<class Wnd>
class Border : public Wnd {
public:
	using Wnd::Wnd;
protected:
	int border;
protected:
	virtual void OnSize() override {
		cout << "Border Size " << border << endl;
		Wnd::OnSize();
	}
};



template<class Wnd, template<class Wnd> class Frame, template<class Wnd> class... Frames>
class Enframe : public Enframe<Frame<Wnd>, Frames...> {
public:
	using Enframe<Frame<Wnd>, Frames...>::Enframe;
	using Base = Enframe;
};


template<class Wnd, template<class Wnd> class Frame>
class Enframe<Wnd, Frame> : public Frame<Wnd> {
public:
	using Frame<Wnd>::Frame;
	using Base = Enframe;
};


//template<class Wnd, template<class Wnd> class Frame1, template<class Frame1> class Frame2>
//class Enframe : public Frame2<Frame1<Wnd>> {
//public:
//	using Frame2<Frame1<Wnd>>::Frame2;
//};


template<class Wnd>
class MyWnd : public Enframe<Wnd, Background, Border> {
public:
	using Base = Enframe<Wnd, Background, Border>;
public:
	using Base::Base;
public:
	MyWnd() : Base(0) {
		this->background = 1;
		this->border = 3;
	}
};


int main() {
	MyWnd<WndObject> mywnd;
}
