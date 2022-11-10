#pragma once

#include "opencv2\core\mat.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
#include "OpenCLData.h"

enum class ImageType {
	//ARGB_32bbp,
	RGB_24bpp,
	Greyscale_16bpp
};

int ImageTypeToGDIPlus(ImageType imageType);
int ImageTypeToOpenCV(ImageType imageType);

ImageType GDIPlusToImageType(int gdiplus);
ImageType OpenCVToImageType(int openCV);

int getBytesPerPixel(ImageType imageType);

class SharedImage {

public:

	SharedImage(int width, int height, ImageType imageType);
	SharedImage(int width, int height, ImageType imageType, OpenCLInfo* openCLInfo);
	SharedImage(cv::Mat* mat, ImageType imageType);
	SharedImage(cv::Mat* mat, ImageType imageType, OpenCLInfo* openCLInfo);
	//SharedImage(Gdiplus::Image* image);

	cv::Mat* getMat();
	Gdiplus::Image* getImage();
	OpenCLMatrix* getOpenCLMatrix();


private:

	int makeOpenCLMatrix(OpenCLInfo* openCLInfo, int memFlags);

	ImageType imageType;

	cv::Mat* mat;
	Gdiplus::Image* image;
	OpenCLMatrix* clImage;

};
