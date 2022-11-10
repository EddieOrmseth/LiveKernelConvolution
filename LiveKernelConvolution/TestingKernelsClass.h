#pragma once

void testKernel(void (*kernelCaller)(int* globalSizes), int workDim, int* globalSizes);

//void testOpenCLKernel(void (*kernel)(), int workDim, int* globalSize);

void ConvolutionOperatorFunc(unsigned char* input, unsigned int inWidth, float* kernelIN, unsigned int kernelSize, unsigned char* output, unsigned int outWidth, unsigned int numChannels, unsigned int stride, unsigned int convPerSector);

void ConvolutionOperatorFunc(unsigned char* input, unsigned int inWidth, float* kernelIN, unsigned int kernelSize, unsigned char* output, unsigned int outWidth, unsigned int numChannels, unsigned int stride, unsigned int convPerSector, int* gID);
