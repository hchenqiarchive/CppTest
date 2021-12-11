


struct ImageRepeatFigure : Figure {
	const Image& image;
	Rect region;
	uchar opacity;

	ImageRepeatFigure(const Image& image, Rect region, uchar opacity = 0xFF) : image(image), region(region), opacity(opacity) {}

	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


void ImageRepeatFigure::DrawOn(RenderTarget& target, Point point) const {
	Size image_size = image.GetSize();
	Size target_size = GetTargetSize(target);
	Rect region_on_target = Rect(point_zero, target_size).Intersect(Rect(point_zero + offset, region.size));
	Vector target_to_image_space_conversion = (region.point - point_zero + offset_on_image) - offset;
	Rect region_on_image_space = region_on_target + target_to_image_space_conversion;
	Rect repeat_range = RegionToOverlappingTileRange(region_on_image_space, image_size);
	if (repeat_range.IsEmpty()) { return; }
	ImageFigure image_figure(image, opacity);
	for (RectPointIterator it(repeat_range); !it.Finished(); ++it) {
		Vector tile_offset = ScalePointBySize(it.Item(), image_size) - point_zero;
		Rect drawing_region_on_image_space = Rect(point_zero + tile_offset, image_size).Intersect(region_on_image_space);
		image_figure.region_on_image = drawing_region_on_image_space - tile_offset;
		image_figure.DrawOn(target, drawing_region_on_image_space.point - point_zero - target_to_image_space_conversion);
	}
}
