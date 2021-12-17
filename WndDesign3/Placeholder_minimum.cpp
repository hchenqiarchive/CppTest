#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class Placeholder;


template<>
class Placeholder<Auto, Auto> : public WndType<Auto, Auto> {
public:
	Placeholder(Size size) : size(size) {}
private:
	Size size;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { return size; }
public:
	void SetSize(Size size) { if (this->size != size) { this->size = size; SizeUpdated(size); } }
};


template<>
class Placeholder<Relative, Auto> : public WndType<Relative, Auto> {
public:
	Placeholder(uint height) : height(height) {}
private:
	uint height;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { return Size(length_min, height); }
public:
	void SetHeight(uint height) { if (this->height != height) { this->height = height; SizeUpdated(Size(length_min, height)); } }
};


template<>
class Placeholder<Auto, Relative> : public WndType<Auto, Relative> {
public:
	Placeholder(uint width) : width(width) {}
private:
	uint width;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { return Size(width, length_min); }
public:
	void SetWidth(uint width) { if (this->width != width) { this->width = width; SizeUpdated(Size(width, length_min)); } }
};


template<>
class Placeholder<Relative, Relative> : public WndType<Relative, Relative> {};


END_NAMESPACE(WndDesign)