#include "SharedImage.hpp"

int ImageTypeToGDIPlus(ImageType imageType) {
	switch (imageType) {
	case ImageType::RGB_24bpp:
		return PixelFormat24bppRGB;
	case ImageType::Greyscale_16bpp:
		return PixelFormat16bppGrayScale;
	}

	throw "No GDIPlus Pixel Format for that ImageType\n";
}

int ImageTypeToOpenCV(ImageType imageType) {
	switch (imageType) {
	case ImageType::RGB_24bpp:
		return CV_8UC3;
	case ImageType::Greyscale_16bpp:
		return CV_16UC1;
	}

	throw "No OpenCV Pixel Format for that ImageType\n";
}

ImageType GDIPlusToImageType(int gdiplus) {
	switch (gdiplus) {
	case PixelFormat24bppRGB:
		return ImageType::RGB_24bpp;
	case PixelFormat16bppGrayScale:
		return ImageType::Greyscale_16bpp;
	}

	throw "No ImageType for that GDIPlus Pixel Format\n";
}

ImageType OpenCVToImageType(int openCV) {
	switch (openCV) {
	case CV_8UC3:
		return ImageType::RGB_24bpp;
	case CV_16UC1:
		return ImageType::Greyscale_16bpp;
	}

	throw "No ImageType for that OpenCV Pixel Format\n";
}

int getBytesPerPixel(ImageType imageType) {
	switch (imageType) {
	case ImageType::RGB_24bpp:
		return 3;
	case ImageType::Greyscale_16bpp:
		return 2;
	}

	throw "Cannot Find Bytes per Pixel for that ImageType\n";
}

SharedImage::SharedImage(int width, int height, ImageType imageType) {
	this->imageType = imageType;
	mat = new cv::Mat(height, width, ImageTypeToOpenCV(imageType));
	image = new Gdiplus::Bitmap(width, height, mat->step1(), ImageTypeToGDIPlus(imageType), mat->data);
}

SharedImage::SharedImage(int width, int height, ImageType imageType, OpenCLInfo* openCLInfo) : SharedImage(width, height, imageType) {
	makeOpenCLMatrix(openCLInfo, CL_MEM_READ_WRITE);
}

SharedImage::SharedImage(cv::Mat* mat, ImageType imageType) {
	this->imageType = imageType;
	this->mat = mat;
	image = new Gdiplus::Bitmap(mat->cols, mat->rows, mat->step1(), ImageTypeToGDIPlus(imageType), mat->data);
}

SharedImage::SharedImage(cv::Mat* mat, ImageType imageType, OpenCLInfo* openCLInfo) : SharedImage(mat, imageType) {
	makeOpenCLMatrix(openCLInfo, CL_MEM_READ_WRITE);
}

int SharedImage::makeOpenCLMatrix(OpenCLInfo* openCLInfo, int memFlags) {
	int result = 0;
	clImage = createOpenCLMatrixHp(mat->rows, mat->cols, getBytesPerPixel(imageType), *openCLInfo, memFlags, &result);
	free(clImage->data.hostData);
	clImage->data.hostData = mat->data;
	return result;
}

//SharedImage::SharedImage(Gdiplus::Image* image) {
//	this->image = image;
//	//mat = new cv::Mat(image->GetWidth(), image->GetHeight(), ImageTypeToOpenCV(GDIPlusToImageType(image->GetPixelFormat())));
//	mat = new cv::Mat(image->GetWidth(), image->GetHeight(), , image->);
//}

cv::Mat* SharedImage::getMat() {
	return mat;
}

Gdiplus::Image* SharedImage::getImage() {
	return image;
}

OpenCLMatrix* SharedImage::getOpenCLMatrix() {
	return clImage;
}