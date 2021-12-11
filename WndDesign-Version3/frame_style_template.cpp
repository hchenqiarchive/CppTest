

// style inheritance with type traits

// enframe test

// frame test
#include <iostream>

#include <type_traits>


using namespace std;


struct Base {
	virtual void f() { cout << "Base" << endl; }
};

struct Frame : Base {
	struct Style {};
	Base* child;
	virtual void f() { cout << "Frame with child "; child->f(); cout << endl; }
	void SetChild(Base& base) {}
};

struct Layout : Base {
	//struct Style {};
	virtual void f() { cout << "Layout"; cout << endl; }
	void AddChild(Base& base) { cout << "Add Child" << endl; }
};

struct OverlapLayout : Base {
	void AddChild(Frame& frame) {}
};

/*
template<class Frame, class Layout>
struct Enframe : Frame, Layout {
	Enframe() {
		static_cast<Frame&>(*this).SetChild(static_cast<Layout&>(*this));
	}
	Frame& GetFrame() const { return *this; }
};
*/

template<class Frame, class Layout>
struct Enframe : Layout {

	template<class Layout, class = void>
	struct StyleHelper : Frame::Style {};

	template<class Layout>
	struct StyleHelper<Layout, void_t<typename Layout::Style>> : Frame::Style, Layout::Style {};

	using Style = StyleHelper<Layout>;


	Frame frame;
	Enframe(Style& style) {
		frame.SetChild(*this);
	}
	Frame& GetFrame() { return frame; }
	operator Frame& () { return frame; }
};


using FramedLayout = Enframe<Frame, Layout>;


FramedLayout::Style style;


struct Wnd : public FramedLayout {
	Wnd() : FramedLayout(style) {}
};


int main() {
	OverlapLayout parent;
	Wnd child;
	parent.AddChild(child.GetFrame());
}



// short version
#include <type_traits>

struct Frame {
	struct Style{};
};

struct Layout {
	struct Style {};
};

template<class Layout, class = void>
struct StyleHelper : Frame::Style {};

template<class Layout>
struct StyleHelper<Layout, std::void_t<typename Layout::Style>> : Frame::Style, Layout::Style {};

using Style = StyleHelper<Layout>;