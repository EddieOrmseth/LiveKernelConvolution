#include "FileImageSource.hpp"
#include "SharedImage.hpp"

FileImageSource::FileImageSource(cv::Mat* mat) {
	this->mat = mat;
}

int FileImageSource::getImage(SharedImage* sharedImage) {
	memcpy(sharedImage->getMat()->data, mat->data, mat->cols * mat->rows * mat->channels() * sizeof(char));
	return 0;
}

int FileImageSource::update() {
	return 0;
}