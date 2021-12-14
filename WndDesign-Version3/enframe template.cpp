

struct Object {};

using ptr = Object*;


struct Control : Object {
	struct Style { int control; };
	Control(Style) {}
};


struct Layout : Object {
	struct Style { int layout; };
	Layout(Style, ptr, ptr) {}
};


struct Frame1 : Object {
	struct Style { int frame1; };
	Frame1(Style, ptr) {}
};

struct Frame2 : Object {
	struct Style { int frame2; };
	Frame2(Style, ptr) {}
};


//template<class... Ts>
//struct select_last {
//	template<class T>
//	struct tag {
//		using type = T;
//	};
//	using type = typename decltype((tag<Ts>{}, ...))::type;
//};


template<class Frame, class... Frames>
struct Combine : Combine<Frames...> {
	struct Style : Frame::Style, Combine<Frames...>::Style {};
	Combine(Style style, ptr ptr) : Combine<Frames...>(style, new Frame(style, ptr)) {}
};

template<class Frame>
struct Combine<Frame> : Frame {
	using Frame::Frame;
};


template<class Wnd, class... Frames>
struct Enframe : Combine<Frames...> {
	struct Style : Wnd::Style, Combine<Frames...>::Style {};

	template<class... Ptr>
	Enframe(Style style, Ptr...ptrs) : Combine<Frames...>(style, new Wnd(style, ptrs...)) {}

	using Base = Enframe;
};


struct MyLayout : public Enframe<Layout, Frame1, Frame2> {
	struct Style : Base::Style {
		Style() {
			layout = 1;
			frame1 = 2;
			frame2 = 3;
		}
	};

	MyLayout() : Base(Style(), new Object, new Object) {}
};


int main() {
	MyLayout mylayout;
}