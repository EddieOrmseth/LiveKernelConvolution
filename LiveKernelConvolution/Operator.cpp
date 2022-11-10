#include "Operator.hpp"

Operator::Operator(SharedImage* input, SharedImage* output) {
	this->input = input;
	this->output = output;
	finished = false;
	running = false;
}

SharedImage* Operator::getInput() {
	return input;
}

SharedImage* Operator::getOutput() {
	return output;
}

void Operator::start() {
	finished = false;
	running = true;
	startOperation();
}

void Operator::finishOperation() {
	running = false;
	finished = true;
}

bool Operator::isRunning() {
	return running;
}

bool Operator::isFinished() {
	return finished;
}

void Operator::resetFinished() {
	finished = false;
}