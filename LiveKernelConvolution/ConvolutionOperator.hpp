#pragma once
#include "Operator.hpp"
#include "SharedImage.hpp"
#include "OpenCLStuff.h"

class Kernel;

class ConvolutionInfo {

public:

	ConvolutionInfo(int iWidth, int iHeight, int kernelSize, int stride, int numChannels, int numSectors, OpenCLInfo* openCLInfo);
	//ConvolutionInfo(OpenCLBuffer* imgKernel, int kernelDim, int numSectors, int convPerSector, int numChannels);

	// Provided
	int iWidth;
	int iHeight;
	int kernelSize;
	int stride;
	int numChannels;
	int numSectors;

	// Calculated
	int oWidth;
	int oHeight;
	int convPerSect;
	int sectorLength;

	OpenCLInfo* openCLInfo;
	//OpenCLBuffer* imgKernel;
	Kernel* imgKernel;
};

class ConvolutionOperator : public Operator {

public:

	ConvolutionOperator(SharedImage* input, SharedImage* output, ConvolutionInfo* convolutionInfo, cl_kernel kernel1);
	ConvolutionOperator(SharedImage* input, SharedImage* output, ConvolutionInfo* convolutionInfo, const char* NameOfKernel);

	void setKernelFinished();

	void startOperation();

	bool isFinished();

private:

	int result = 0;

	bool kernelFinished = true;
	cl_kernel kernel;
	cl_event callback;
	OpenCLInfo* openCLInfo;

	cl_event afterRead;
	cl_event afterAlg;

	size_t* workOffset;
	size_t* globalWorKSize;
	size_t* localWorKSize;

};

