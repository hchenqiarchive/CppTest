
using uint = unsigned int;

struct Size {
	uint width;
	uint height;
};


struct ListLayout {
	uint Size::* auto_dimension = &Size::height;
	
	void f() {
		sizeof(auto_dimension);
	}

	bool is_vertical;  // true: vertical, false: horizontal

	uint Size::* GetActualDimension() {
		return is_vertical ? &Size::width : &Size::height;
	}
	uint Size::* GetAutoDimension() {
		return is_vertical ? &Size::height : &Size::width;
	}

	const Size UpdateSizeRef(Size size_ref) {
		uint Size::* actual_dimension = GetActualDimension();
		uint Size::* auto_dimension = GetAutoDimension();

		Size child_size_ref; 
		child_size_ref.*actual_dimension = size_ref.*actual_dimension;
		child_size_ref.*auto_dimension = 0;

		return child_size_ref;
	}
};


int main() {
	ListLayout().UpdateSizeRef(Size{ 5, 10 });
}