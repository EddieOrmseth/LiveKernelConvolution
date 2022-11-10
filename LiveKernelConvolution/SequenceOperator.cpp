#include "SequenceOperator.hpp"

SequenceOperator::SequenceOperator(Operator* operations, int numOperations) : Operator(operations[0].getInput(), operations[numOperations - 1].getOutput()) {
	this->operations = operations;
	this->numOperations = numOperations;
}

void SequenceOperator::startOperation() {
	for (int i = 0; i < numOperations; i++) {
		operations[i].resetFinished();
	}

	operations[0].start();
}

bool SequenceOperator::isFinished() {
	for (int i = 0; i < numOperations - 1; i++) {
		if (operations[i].isFinished() && !operations[i + 1].isFinished() && !operations[i + 1].isRunning()) {
			operations[i + 1].start();
			return false;
		}
	}

	//finished = operations[numOperations - 1].isFinished();
	//return Operator::isFinished();
	return operations[numOperations - 1].isFinished();
}
