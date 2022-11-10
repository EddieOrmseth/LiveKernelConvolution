#pragma once
#include "GraphicsEngine.hpp"

class ImageDisplay : public Paintable {

public:

	ImageDisplay(Gdiplus::Image* image);

	void paint(Gdiplus::Graphics* graphics);

private:

	Gdiplus::Image* image;

};
