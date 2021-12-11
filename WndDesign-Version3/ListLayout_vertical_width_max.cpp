
class ListLayoutVerticalWidthMax : public WndObject {
private:
	struct ChildInfo {
		ref_ptr<WndObject> child;
		Size size;
		uint y;
	};
	std::vector<ChildInfo> child_list;


private:
	void SetChildData(WndObject& child, uint64 index) {
		WndObject::SetChildData<uint64>(child, index);
	}
	uint64 GetChildData(WndObject& child) {
		return WndObject::GetChildData<uint64>(child);
	}


public:
	void AppendChild(WndObject& child) {
		AddChild(child);
		SetChildData(child, child_list.size());
		ChildInfo& info = child_list.emplace_back(&child, 0, 0);
		info.y = size.height;
		info.size = SetChildSizeRef(child, Size(width_max, length_min));
		size.height += info.size.height;
		size.width = min(width_max, max(size.width, info.size.width));
		SizeChanged(size);
	}


private:
	Size size;

private:
	uint width_max;


	virtual const Size OnSizeRefChange(Size size_ref) override {
		if (width_max != size_ref.width) {
			width_max = size_ref.width;
			size = size_empty;
			for (auto& info : child_list) {
				Size child_size = SetChildSizeRef(*info.child, Size(width_max, length_min));
				info.size = Size(min(width_max, child_size.width), child_size.height);
				info.y = size.height;
				size.height += info.size.height;
				size.width = max(size.width, info.size.width);
			}
		}
		return size;
	}

	virtual void OnChildSizeChange(WndObject& child, Size child_size) {
		uint64 index = GetChildData(child); assert(index < child_list.size());
		ChildInfo& info = child_list[index];
		bool size_changed = false;
		info.size.width = child_size.width;
		if (child_size.width > size.width && size.width < width_max) {
			size.width = min(child_size.width, width_max);
			size_changed = true;
		} else if (child_size.width < size.width) {
			uint child_width_max = 0;
			for (auto& info : child_list) {
				child_width_max = max(child_width_max, info.size.width);
			}
			if (child_width_max < size.width) {
				size.width = child_width_max;
				size_changed = true;
			}
		}
		if (info.size.height != child_size.height) {
			info.size.height = child_size.height;
			size.height = info.y;
			for (; index < child_list.size(); index++) {
				child_list[index].y = size.height;
				size.height += child_list[index].size.height;
			}
			size_changed = true;
		}
		if (size_changed) {
			SizeChanged(size);
		}
	}
};


template<uint row_height>
class ListLayoutVerticalWidthActualRowHeightFixed : public WndObject {
	struct ChildInfo {
		ref_ptr<WndObject> child;
	};
	std::vector<ChildInfo> child_list;

private:
	uint width;

	virtual const Size GetSize() const override {
		return Size(width, row_height);
	}
};