#pragma once
#include "ImageSource.hpp"
#include "opencv2\opencv.hpp"

class OCVVideoCapture : public ImageSource {

public:

	OCVVideoCapture(cv::VideoCapture* videoCapture, int index, bool isLive);
	~OCVVideoCapture();

	int getImage(SharedImage* sharedImage);

	int update();

private:

	int index;
	cv::VideoCapture* videoCapture;
	bool isLive;

};

