#include "ConvolutionOperator.hpp"
#include "Kernel.hpp"

ConvolutionInfo::ConvolutionInfo(int iWidth, int iHeight, int kernelSize, int stride, int numChannels, int numSectors, OpenCLInfo* openCLInfo) {
	this->iWidth = iWidth;
	this->iHeight = iHeight;
	this->kernelSize = kernelSize;
	this->stride = stride;
	this->numChannels = numChannels;
	this->numSectors = numSectors;

	oWidth = 1 + ((iWidth - kernelSize) / stride);
	oWidth = (oWidth / numSectors) * numSectors;
	oHeight = 1 + ((iHeight - kernelSize) / stride);
	convPerSect = oWidth / numSectors;
	sectorLength = convPerSect * stride;

	// FUCK MICROSOFT!
	if (oWidth % 4 != 0) {
		oWidth += 4 - (oWidth % 4);
	}
	// END RAGE

	int result = 0;
	this->openCLInfo = openCLInfo;
	imgKernel = new Kernel(kernelSize, numChannels, openCLInfo);
}

void notifyFinishedFunc(cl_event theEvent, cl_int command_status, void* data) {
	if (command_status == CL_COMPLETE) {
		((ConvolutionOperator*)data)->setKernelFinished();
	}
}

ConvolutionOperator::ConvolutionOperator(SharedImage* input, SharedImage* output, ConvolutionInfo* convolutionInfo, cl_kernel kernel) : Operator(input, output) {
	this->kernel = kernel;
	this->openCLInfo = convolutionInfo->openCLInfo;

	result |= clSetKernelArg(kernel, 0, sizeof(input->getOpenCLMatrix()->data.buffer), &input->getOpenCLMatrix()->data.buffer); // Input Data
	result |= clSetKernelArg(kernel, 1, sizeof(convolutionInfo->iWidth), &convolutionInfo->iWidth); // Input Cols
	result |= clSetKernelArg(kernel, 2, sizeof(convolutionInfo->imgKernel->getKernelData()->data.buffer), &convolutionInfo->imgKernel->getKernelData()->data.buffer); // Kernel Data
	result |= clSetKernelArg(kernel, 3, sizeof(convolutionInfo->kernelSize), &convolutionInfo->kernelSize); // Kernel Dim
	result |= clSetKernelArg(kernel, 4, sizeof(output->getOpenCLMatrix()->data.buffer), &output->getOpenCLMatrix()->data.buffer); // Output Data
	result |= clSetKernelArg(kernel, 5, sizeof(convolutionInfo->oWidth), &convolutionInfo->oWidth); // Output Cols
	result |= clSetKernelArg(kernel, 6, sizeof(convolutionInfo->numChannels), &convolutionInfo->numChannels); // Num Channels
	result |= clSetKernelArg(kernel, 7, sizeof(convolutionInfo->stride), &convolutionInfo->stride); // Stride
	result |= clSetKernelArg(kernel, 8, sizeof(convolutionInfo->convPerSect), &convolutionInfo->convPerSect); // convPerSector

	workOffset = (size_t*)malloc(sizeof(size_t) * 2); workOffset[0] = 0; workOffset[1] = 0;
	globalWorKSize = (size_t*)malloc(sizeof(size_t) * 2); globalWorKSize[0] = output->getMat()->rows; globalWorKSize[1] = convolutionInfo->numSectors;
	localWorKSize = (size_t*)malloc(sizeof(size_t) * 2); localWorKSize[0] = 1; localWorKSize[1] = 1;
}

ConvolutionOperator::ConvolutionOperator(SharedImage* input, SharedImage* output, ConvolutionInfo* convolutionInfo, const char* NameOfKernel) : Operator(input, output) {
	this->openCLInfo = convolutionInfo->openCLInfo;

	kernel = clCreateKernel(openCLInfo->program, NameOfKernel, &result);

	result |= clSetKernelArg(kernel, 0, sizeof(input->getOpenCLMatrix()->data.buffer), &input->getOpenCLMatrix()->data.buffer); // Input Data
	result |= clSetKernelArg(kernel, 1, sizeof(convolutionInfo->iWidth), &convolutionInfo->iWidth); // Input Cols
	result |= clSetKernelArg(kernel, 2, sizeof(convolutionInfo->imgKernel->getKernelData()->data.buffer), &convolutionInfo->imgKernel->getKernelData()->data.buffer); // Kernel Data
	result |= clSetKernelArg(kernel, 3, sizeof(convolutionInfo->kernelSize), &convolutionInfo->kernelSize); // Kernel Dim
	result |= clSetKernelArg(kernel, 4, sizeof(output->getOpenCLMatrix()->data.buffer), &output->getOpenCLMatrix()->data.buffer); // Output Data
	result |= clSetKernelArg(kernel, 5, sizeof(convolutionInfo->oWidth), &convolutionInfo->oWidth); // Output Cols
	result |= clSetKernelArg(kernel, 6, sizeof(convolutionInfo->numChannels), &convolutionInfo->numChannels); // Num Channels
	result |= clSetKernelArg(kernel, 7, sizeof(convolutionInfo->stride), &convolutionInfo->stride); // Stride
	result |= clSetKernelArg(kernel, 8, sizeof(convolutionInfo->convPerSect), &convolutionInfo->convPerSect); // convPerSector

	workOffset = (size_t*)malloc(sizeof(size_t) * 2); workOffset[0] = 0; workOffset[1] = 0;
	globalWorKSize = (size_t*)malloc(sizeof(size_t) * 2); globalWorKSize[0] = output->getMat()->rows; globalWorKSize[1] = convolutionInfo->numSectors;
	localWorKSize = (size_t*)malloc(sizeof(size_t) * 2); localWorKSize[0] = 1; localWorKSize[1] = 1;
}

void ConvolutionOperator::setKernelFinished() {
	kernelFinished = true;
	framesCompleted++;
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	totalMilliseconds += std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
	finishOperation();
}

void ConvolutionOperator::startOperation() {
	kernelFinished = false;
	startTime = std::chrono::high_resolution_clock::now();

	result |= clEnqueueWriteBuffer(openCLInfo->commandQueue, input->getOpenCLMatrix()->data.buffer, CL_FALSE, 0, input->getOpenCLMatrix()->data.bufferRawSize, input->getOpenCLMatrix()->data.hostData, 0, 0, &afterRead);
	result |= clEnqueueNDRangeKernel(openCLInfo->commandQueue, kernel, 2,  workOffset, globalWorKSize, localWorKSize, 1, &afterRead, &afterAlg);
	result |= clEnqueueReadBuffer(openCLInfo->commandQueue, output->getOpenCLMatrix()->data.buffer, CL_FALSE, 0, output->getOpenCLMatrix()->data.bufferRawSize, output->getOpenCLMatrix()->data.hostData, 1, &afterAlg, &callback);

	result |= clSetEventCallback(callback, CL_COMPLETE, notifyFinishedFunc, this);
}

bool ConvolutionOperator::isFinished() {
	return Operator::isFinished();
}

int ConvolutionOperator::getFramesCompleted() {
	return framesCompleted;
}

long long ConvolutionOperator::getTotalMilliseconds() {
	return totalMilliseconds;
}
