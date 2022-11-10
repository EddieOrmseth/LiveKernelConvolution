#include "ImageDisplay.hpp"

ImageDisplay::ImageDisplay(Gdiplus::Image* image) {
	this->image = image;
}

void ImageDisplay::paint(Gdiplus::Graphics* graphics) {
	graphics->DrawImage(image, 0, 0, image->GetWidth(), image->GetHeight());
}
