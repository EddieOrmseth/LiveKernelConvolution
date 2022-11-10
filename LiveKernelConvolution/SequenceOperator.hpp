#pragma once
#include "Operator.hpp"

class SequenceOperator : public Operator {

public:

	SequenceOperator(Operator* operations, int numOperations);

	virtual void startOperation();

	virtual bool isFinished();

private:

	int numOperations;
	Operator* operations;
	
};

