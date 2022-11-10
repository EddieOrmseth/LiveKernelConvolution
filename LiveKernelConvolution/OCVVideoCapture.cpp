#include "OCVVideoCapture.hpp"
#include "SharedImage.hpp"

OCVVideoCapture::OCVVideoCapture(cv::VideoCapture* videoCapture, int index, bool isLive) {
	this->videoCapture = videoCapture;
	this->index = index;
	this->isLive = isLive;
}

OCVVideoCapture::~OCVVideoCapture() {
	delete videoCapture;
}

int OCVVideoCapture::getImage(SharedImage* sharedImage) {
	if (!videoCapture->isOpened()) {
		videoCapture->open(index);
	}

	//std::cout << "Image Pulled\n";
	bool success = videoCapture->read(*sharedImage->getMat());

	return 0;
}

int OCVVideoCapture::update() {
	if (isLive) {
		videoCapture->grab();
	}

	return 0;
}
