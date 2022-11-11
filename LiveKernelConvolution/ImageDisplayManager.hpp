#pragma once
#include <mutex>
#include <queue>
#include <thread>
#include <functional>
#include "GraphicsEngine.hpp"

class Operator;
class ImageSource;
class SharedImage;

class ImageDisplayManager : public Paintable {

public:

	friend void handlerOperatorsData(ImageDisplayManager* manager);
	friend void handlerOperatorsProcess(ImageDisplayManager* manager);

	ImageDisplayManager(ImageSource* imageSource, std::vector<Operator*> operators);
	~ImageDisplayManager();

	void paint(Gdiplus::Graphics* graphics);

	void startHandleThreads();

	void upate();

	double getOverallFps();

private:

	std::vector<Operator*> operators;
	ImageSource* imageSource;

	bool run = false;
	std::thread dataHandleThread;
	std::thread processHandleThread;

	std::mutex* dataMutex;
	std::queue<Operator*> dataQueue;
	std::mutex* processMutex;
	std::queue<Operator*> processQueue;
	std::mutex* displayMutex;
	std::queue<Operator*> displayQueue;

	int framesCompleted;
	std::chrono::high_resolution_clock::time_point startTime;

};
