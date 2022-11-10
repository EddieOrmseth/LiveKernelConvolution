#include "ImageDisplayManager.hpp"
#include "Operator.hpp"
#include "ImageSource.hpp"
#include "SharedImage.hpp"
#include "opencv2\opencv.hpp"

ImageDisplayManager::ImageDisplayManager(ImageSource* imageSource, std::vector<Operator*> operators) {
	this->imageSource = imageSource;
	this->operators = operators;
	dataMutex = new std::mutex();
	dataQueue = std::queue<Operator*>();
	processMutex = new std::mutex();
	processQueue = std::queue<Operator*>();
	displayMutex = new std::mutex();
	displayQueue = std::queue<Operator*>();

	dataMutex->lock();
	for (int i = 0; i < operators.size(); i++) {
		dataQueue.push(operators.at(i));
	}
	dataMutex->unlock();
}

ImageDisplayManager::~ImageDisplayManager() {
	run = false;
	dataHandleThread.join();
	processHandleThread.join();
}

void ImageDisplayManager::paint(Gdiplus::Graphics* graphics) {
	if (displayQueue.size() > 0) {
		displayMutex->lock();
		Operator* op = displayQueue.front();
		Gdiplus::Image* imageIN = op->getInput()->getImage();
		Gdiplus::Image* imageOUT = op->getOutput()->getImage();
		if (displayQueue.size() > 1) {
			displayQueue.pop();
			displayMutex->unlock();
			graphics->DrawImage(imageIN, 0, 0, imageIN->GetWidth(), imageIN->GetHeight());
			graphics->DrawImage(imageOUT, imageIN->GetWidth(), 0, imageOUT->GetWidth(), imageOUT->GetHeight());
			dataMutex->lock();
			dataQueue.push(op);
			dataMutex->unlock();
		}
		else {
			displayMutex->unlock();
			graphics->DrawImage(imageIN, 0, 0, imageIN->GetWidth(), imageIN->GetHeight());
			graphics->DrawImage(imageOUT, imageIN->GetWidth(), 0, imageOUT->GetWidth(), imageOUT->GetHeight());
		}

	}
}

void handlerOperatorsData(ImageDisplayManager* manager) {
	while (manager->run) {
		if (manager->dataQueue.size() > 0) {
			manager->dataMutex->lock();
			Operator* op = manager->dataQueue.front();
			if (op != nullptr) {
				manager->dataQueue.pop();
			}
			manager->dataMutex->unlock();

			if (op != nullptr) {
				manager->imageSource->getImage(op->getInput());
				//std::cout << op->isFinished() << std::endl;
				op->start();
				//std::cout << op->isFinished() << std::endl;
				manager->processMutex->lock();
				manager->processQueue.push(op);
				manager->processMutex->unlock();
			}
		}
	}
}

void handlerOperatorsProcess(ImageDisplayManager* manager) {
	int i = 0;
	bool finished = false;
	while (manager->run) {
		if (manager->processQueue.size() > 0) {
			manager->processMutex->lock();
			Operator* op = manager->processQueue.front();
			if (op != nullptr) {
				finished = op->isFinished();
				if (finished) {
					manager->processQueue.pop();
				}
			}
			manager->processMutex->unlock();

			if (op != nullptr && finished) {
				i++;

				manager->displayMutex->lock();
				manager->displayQueue.push(op);
				manager->displayMutex->unlock();
			}
		}
	}
}

void ImageDisplayManager::startHandleThreads() {
	run = true;
	dataHandleThread = std::thread(handlerOperatorsData, this);
	processHandleThread = std::thread(handlerOperatorsProcess, this);
}

//void ImageDisplayManager::handleSources() {
//	//handleSourcesFunc(this);
//}

void ImageDisplayManager::upate() {
	for (int i = 0; i < operators.size(); i++) {
		operators[i]->isFinished();
	}
}
