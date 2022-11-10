#pragma once
#include "SharedImage.hpp"

class Operator {

public:

	Operator(SharedImage* input, SharedImage* output);

	SharedImage* getInput();
	SharedImage* getOutput();

	void start();
	virtual void startOperation() = 0; // implemented by derived class

	void finishOperation(); // called by dervied class at end of operation

	bool isRunning();

	virtual bool isFinished();
	void resetFinished();

protected:

	SharedImage* input;
	SharedImage* output;

private:

	bool running;
	bool finished;

};
