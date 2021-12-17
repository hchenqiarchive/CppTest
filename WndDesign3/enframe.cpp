


template<class FrameType, class WidthType, class HeightType>
child_ptr<WidthType, HeightType> Enframe(child_ptr<WidthType, HeightType> child) {
	class Frame : public FrameType, public LayoutType<WidthType, HeightType> {
	public:
		using child_ptr = child_ptr<>;
	public:
		Frame(child_ptr child) : FrameType(std::move(child)) {}
	};
	return std::make_unique<Frame>(child);
}