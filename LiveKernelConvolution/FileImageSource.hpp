#pragma once
#include "ImageSource.hpp"
#include "opencv2\opencv.hpp"

class FileImageSource : public ImageSource {

public:

	FileImageSource(cv::Mat* mat);

	int getImage(SharedImage* sharedImage);

	int update();

private:

	cv::Mat* mat;

};

