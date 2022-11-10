#include "Kernels.h"
#include <stdlib.h>

const char* nameOfConvolutionOperator = "ConvolutionOperator";
const char* ConvolutionOperatorVF = "\n" \
"__kernel void ConvolutionOperator(__global unsigned char* input, unsigned int inWidth, __global float* kernelIN, unsigned int kernelSize, __global unsigned char* output, unsigned int outWidth, unsigned int numChannels, unsigned int stride, unsigned int convPerSector) { \n" \
"   //printf(\"Here I am\\n\");                                                                       \n" \
"                                                                                                   \n" \
"	unsigned int oRow = get_global_id(0); // Output Row                                             \n" \
"	//unsigned int oRow = gID[0]; // Output Row                                                     \n" \
"	unsigned int sectID = get_global_id(1); // Input Sector                                         \n" \
"	//unsigned int sectID = gID[1]; // Input Sector                                                 \n" \
"                                                                                                   \n" \
"	unsigned int oCol = sectID * convPerSector;                                                     \n" \
"                                                                                                   \n" \
"	unsigned int iRow = oRow * stride;                                                              \n" \
"	unsigned int iCol = oCol * stride;                                                              \n" \
"                                                                                                   \n" \
"	unsigned int oStartIdx = (oRow * outWidth + oCol) * numChannels;                                \n" \
"	unsigned int iStartIdx = (iRow * inWidth + iCol) * numChannels;                                 \n" \
"                                                                                                   \n" \
"	unsigned int oStopIdx = oStartIdx + convPerSector * numChannels;                                \n" \
"	unsigned int iStopIdx = iStartIdx + convPerSector * stride * numChannels;                       \n" \
"                                                                                                   \n" \
"	unsigned int kerSizeSqrd = kernelSize * kernelSize;                                             \n" \
"	float result = 0;                                                                               \n" \
"	unsigned int k = 0;                                                                             \n" \
"	unsigned int pI = iStartIdx;                                                                    \n" \
"                                                                                                   \n" \
"   bool allZeros = true;                                                                             \n" \
"                                                                                                   \n" \
"	for (unsigned int pO = oStartIdx; pO < oStopIdx; pO += numChannels) { // OUT: Pixel to Pixel    \n" \
"	    unsigned int pIU = pI;                                                                      \n" \
"                                                                                                   \n" \
"	    for (unsigned int c = 0; c < numChannels; c++) { // Each Channel                            \n" \
"	        result = 0;                                                                             \n" \
"	        k = 0;                                                                                  \n" \
"	        pIU = pI + c;                                                                           \n" \
"                                                                                                   \n" \
"	        for (unsigned int kY = 0; kY < kernelSize; kY++) { // Iterate over Y                    \n" \
"                                                                                                   \n" \
"	            for (unsigned int kX = 0; kX < kernelSize; kX++) { // Iterate over X                \n" \
"		             float kVal = kernelIN[k + c * kerSizeSqrd];                                    \n" \
"                    //printf(\"%f \", kVal);                                                         \n" \
"			         unsigned int iVal = input[pIU];                                                \n" \
"                    //printf(\"%i \", iVal);                                                         \n" \
"				     result += kVal * iVal;                                                         \n" \
"                                                                                                   \n" \
"	                pIU += numChannels;                                                             \n" \
"	                k++;                                                                            \n" \
"	            }                                                                                   \n" \
"                                                                                                   \n" \
"	            pIU += (inWidth - kernelSize) * numChannels;                                        \n" \
"	        }                                                                                       \n" \
"                                                                                                   \n" \
"	        if (result > 255) { result = 255; }                                                     \n" \
"	        if (result < 0) { result = 0; }                                                         \n" \
"                                                                                                   \n" \
"	        output[pO + c] = result;                                                                \n" \
"           //printf(\"%f \", result);                                                                \n" \
"	    }                                                                                           \n" \
"                                                                                                   \n" \
"	    pI += stride * numChannels;                                                                 \n" \
"	}                                                                                               \n" \
"                                                                                                   \n" \
"                                                                                                   \n" \
"   //printf(\"Now Exiting\\n\");                                                                     \n" \
"}                                                                                                  \n" \
"\n";

const char* nameOfRGBToGreyscaleOperator = "RGBToGreyscaleOperator";
const char* RGBToGreyscaleOperator = "\n" \
"__kernel void RGBToGreyscaleOperator(__global char* RGBData, __global __int16* greyscaleData, int startIndex, int stopIndex) {  \n" \
"   // Get the index of the current element to be processed                                         \n" \
"   uint gID = get_global_id(0);                                                                    \n" \
"                                                                                                   \n" \
"                                                                                                   \n" \
"                                                                                                   \n" \
"   while () {                                                                                      \n" \
"       //greyscaleData[i]                                                                          \n" \
"   }                                                                                               \n" \
"                                                                                                   \n" \
"}                                                                                                  \n" \
"\n";



const int NumberOfKernels = 1;
const char** kernels;


int initializeKernels() {
	kernels = (const char**)malloc(NumberOfKernels * sizeof(char*));
	kernels[0] = ConvolutionOperatorVF;

	return 0;
}