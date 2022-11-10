#pragma once

extern const int NumberOfKernels;
extern const char** kernels;

extern const char* nameOfConvolutionOperator;

extern const char* nameOfRGBToGreyscaleOperator;
extern const char* nameOfRGBToLABOperator;

int initializeKernels();