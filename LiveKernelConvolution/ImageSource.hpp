#pragma once

class SharedImage;

class ImageSource {

public:

	virtual int getImage(SharedImage* sharedImage) = 0;

	virtual int update() = 0;

};
