#pragma once

#include "../window/wnd_traits.h"

#include <array>
#include <vector>


BEGIN_NAMESPACE(WndDesign)


constexpr uint dynamic_extent = -1;


template<class Direction, uint size>
class _ListLayout_Base : public WndObject {
private:
	template<class Direction> struct child_ptr_selector;
	template<> struct child_ptr_selector<Vertical> { using type = child_ptr<Assigned, Auto>; };
	template<> struct child_ptr_selector<Horizontal> { using type = child_ptr<Auto, Assigned>; };
private:
	template<class Direction> struct layout_type_selector;
	template<> struct layout_type_selector<Vertical> { using type = LayoutType<Assigned, Auto>; };
	template<> struct layout_type_selector<Horizontal> { using type = LayoutType<Auto, Assigned>; };
private:
	template<class T, size_t size> struct array_vector_selector { using type = std::array<T, size>; };
	template<class T> struct array_vector_selector<T, dynamic_extent> { using type = std::vector<T>; };
public:
	_ListLayout_Base(uint gap) : gap(gap) {}
private:
	uint gap;
	Size size;
private:
	struct ChildInfo {
		child_ptr_selector<Direction>::type child;
		uint y = 0;
		uint height = 0;
	};
	array_vector_selector<ChildInfo, size>::type child_list;
private:
	void SetChildData(WndObject& child, uint64 index) { WndObject::SetChildData<uint64>(child, index); }
	uint64 GetChildData(WndObject& child) { return WndObject::GetChildData<uint64>(child); }
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size.width != size_ref.width) {
			size.width = size_ref.width;
			size.height = 0;
			for (auto& info : child_list) {
				info.y = size.height;
				info.height = UpdateChildSizeRef(*info.child, Size(size.width, length_min)).height;
				size.height += info.height + row_gap;
			}
			size.height -= child_list.empty() ? 0 : row_gap;
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		uint64 index = GetChildData(child); assert(index < child_list.size());
		if (child_list[index].height != child_size.height) {
			child_list[index].height = child_size.height;
			size.height = child_list[index].y + child_list[index].height;
			for (index++; index < child_list.size(); index++) {
				size.height += row_gap;
				child_list[index].y = size.height;
				size.height += child_list[index].height;
			}
			SizeUpdated(size);
		}
	}
};


template<class Direction, uint size = dynamic_extent>
class ListLayout : public _ListLayout_Base<Direction, size> {
public:
	ListLayout(std::array<)
};


template<class Direction>
class ListLayout<Direction, dynamic_extent> : public _ListLayout_Base<Direction, size> {
public:
	void AppendChild(child_ptr child) {
		RegisterChild(child);
		SetChildData(child, child_list.size());
		ChildInfo& info = child_list.emplace_back(std::move(child));
		info.y = size.height;
		info.height = UpdateChildSizeRef(info.child, Size(size.width, length_min)).height;
		size.height += info.height;
		SizeUpdated(size);
	}
};


END_NAMESPACE(WndDesign)