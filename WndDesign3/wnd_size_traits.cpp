
class WndObject {

};

enum class WndSizeTrait {  // Or SizeContract ?
	Independent,  // Auto or Fixed
	Assigned,
	Max,
	Relative
};

template<WndSizeTrait width, WndSizeTrait height>
class Wnd : public WndObject {};

using WndWidthAssignedHeightAuto = Wnd<WndSizeTrait::Assigned, WndSizeTrait::Auto>;
using WndWidthMaxHeightAuto = Wnd<WndSizeTrait::Max, WndSizeTrait::Auto>;


class ListLayout : public WndWidthAssignedHeightAuto {
	void AddChild(WndWidthAssignedHeightAuto& child) {}
};

class FlowLayout : public WndWidthMaxHeightAuto {

};


// empty struct version
#include <type_traits>

#include <variant>


struct LayoutTraitBase {

};

struct Fixed {};

struct Auto {};

struct Assigned {};

struct Max {};


using Independent = std::variant<Auto, Fixed>;



template<class Width, class Height, class WndBase = WndObject>
class Wnd : public WndBase {

};


class WndObject {

};


class Layout : public WndObject {
public:
	void AddChild(const Independent& child) {

	}
};


struct Frame {
	void SetChild(Wnd<Assigned, Independent> child) {

	}
};


class ListLayout : public Wnd<Assigned, Auto> {

};


int main() {
	Layout().AddChild(Auto());
	Frame().SetChild(ListLayout());
}



// direction traits
struct Direction{};
struct Horizontal{};
struct Vertical{};



// layout type base class
template<class WidthType>
class ChildWidthType;
template<>
class ChildWidthType<Assigned> {
	void UpdateChildWidthRef(uint width_ref) {}
};
template<>
class ChildWidthType<Relative> {
	uint UpdateChildWidthRef(uint width_ref) {}
	virtual void OnChildWidthUpdate(uint width) {}
};
template<>
class ChildWidthType<Auto> {
	virtual void OnChildWidthUpdate(uint width) {}
};

class WidthType<Assigned> {
	virtual void OnWidthRefUpdate(uint width_ref) {}
};
class WidthType<Relative> {
	virtual uint OnWidthRefUpdate(uint width_ref) {}
	void WidthUpdated(uint width) {}
};
class WidthType<Auto> {
	void WidthUpdated(uint width) {}
};