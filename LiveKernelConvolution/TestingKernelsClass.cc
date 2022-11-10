#include "TestingKernelsClass.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstring>

void testKernel(void (*kernelCaller)(int* globalSizes), int workDim, int* globalSizes) {
    
    bool continueRun = true;
    int* recordedIDs = (int*)malloc(sizeof(int) * workDim);
    memset(recordedIDs, 0, sizeof(int) * workDim);

    while (continueRun) {

        (*kernelCaller)(recordedIDs);

        bool continueCarry;
        int i = workDim - 1;
        recordedIDs[i]++;

        continueCarry = recordedIDs[i] == globalSizes[i];
        while (continueCarry && i > 0) {
            if (i != 0) {
                recordedIDs[i] = 0;
                i--;
                recordedIDs[i]++;
            }

            continueCarry = recordedIDs[i] != globalSizes[i];
        }
        
        continueRun = recordedIDs[0] != globalSizes[0];
    }

    free(recordedIDs);
}

void ConvolutionOperatorFunc(unsigned char* input, unsigned int inWidth, float* kernelIN, unsigned int kernelSize, unsigned char* output, unsigned int outWidth, unsigned int numChannels, unsigned int stride, unsigned int convPerSector) {

    int* gIDs = (int*)malloc(sizeof(int) * 2);
    gIDs[0] = 0;
    gIDs[1] = 0;

    ConvolutionOperatorFunc(input, inWidth, kernelIN, kernelSize, output, outWidth, numChannels, stride, convPerSector, gIDs);

    free(gIDs);
}

void ConvolutionOperatorFunc(unsigned char* input, unsigned int inWidth, float* kernelIN, unsigned int kernelSize, unsigned char* output, unsigned int outWidth, unsigned int numChannels, unsigned int stride, unsigned int convPerSector, int* gID) {
                                                                                                    
    //unsigned int oRow = get_global_id(0); // Output Row                                                     
    unsigned int oRow = gID[0]; // Output Row                                                     
    //unsigned int sectID = get_global_id(1); // Input Sector                                                 
    unsigned int sectID = gID[1]; // Input Sector                                                 
                                                                                                    
    unsigned int oCol = sectID * convPerSector;                                                             
                                                                                                    
    unsigned int iRow = oRow * stride;                                                                      
    unsigned int iCol = oCol * stride;                                                                      
                                                                                                    
    unsigned int oStartIdx = (oRow * outWidth + oCol) * numChannels;                                        
    unsigned int iStartIdx = (iRow * inWidth + iCol) * numChannels;                                         
                                                                                                    
    unsigned int oStopIdx = oStartIdx + convPerSector * numChannels;                                        
    unsigned int iStopIdx = iStartIdx + convPerSector * stride * numChannels;                               
                                                                                                    
    unsigned int kerSizeSqrd = kernelSize * kernelSize;                                                     
    float result = 0;                                                                               
    unsigned int k = 0;                                                                                     
    unsigned int pI = iStartIdx;                                                                            
                                                                                                    
    for (unsigned int pO = oStartIdx; pO < oStopIdx; pO += numChannels) { // OUT: Pixel to Pixel                         
        unsigned int pIU = pI;                                                                              
                                                                                                    
        for (unsigned int c = 0; c < numChannels; c++) { // Each Channel                                    
            result = 0;                                                                             
            k = 0;                                                                                  
            pIU = pI + c;

            for (unsigned int kY = 0; kY < kernelSize; kY++) { // Iterate over Y                            
                                                                                                    
                for (unsigned int kX = 0; kX < kernelSize; kX++) { // Iterate over X                        
                    float kVal = kernelIN[k + c * kerSizeSqrd];                                     
                    unsigned int iVal = input[pIU];                                                         
                    result += kVal * iVal;                                                          
                                                                                                    
                    pIU += numChannels;                                                             
                    k++;                                                                            
                }                                                                                   
                                                                                                    
                pIU += (inWidth - kernelSize) * numChannels;                                        
            }                                                                                       
                                                                                                    
            if (result > 255) { result = 255; }                                                     
            if (result < 0) { result = 0; }                                                         
                                                                                                    
            output[pO + c] = result;                                                                    
        }
                                                                                                    
        pI += stride * numChannels;                                                                 
    }                                                                                               
                                                                                                    
 }                                                                                                  
