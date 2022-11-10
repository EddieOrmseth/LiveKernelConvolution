#include <iostream>
#include "OpenCLData.h"
#include "Kernels.h"
#include "opencv2\opencv.hpp"
#include "TestingKernelsClass.h"
#include <Windows.h>
#include "SharedImage.hpp"
#include "ImageDisplayManager.hpp"
#include "ImageDisplay.hpp"
#include "GraphicsEngine.hpp"
#include "InputRegistry.hpp"
#include "Profiler.hpp"
#include "ConvolutionOperator.hpp"
#include "OCVVideoCapture.hpp"
#include "FileImageSource.hpp"
#include "Kernel.hpp"

void KernelTest();
void DisplayCaptureTest();
void DisplayCaptureTest(SharedImage* img);
void testBlur();
void runProgram();

int main() {
    std::cout << "Hello World!\n";

    //KernelTest();
    //DisplayCaptureTest();
    //testBlur();
    runProgram();

    std::cout << "Goodbye World!\n";
}

unsigned char* hostInput;
cl_int iCols;
float* hostKernel;
cl_int kernelDim;
unsigned char* hostOutput;
cl_int oCols;
cl_int numChannels;
cl_int stride;
cl_int convPerSect;

void testConv(int* globalSizes) {
    ConvolutionOperatorFunc(hostInput, iCols, hostKernel, kernelDim, hostOutput, oCols, numChannels, stride, convPerSect, globalSizes);
}

void KernelTest() {
    // Known
    iCols = 10;
    cl_int iRows = 10;
    kernelDim = 5;
    stride = 1;
    cl_int sectors = 1;
    numChannels = 3;

    // Calculated
    //oCols = 3;
    //oCols = 1 + ((iCols - kernelDim) / stride);
    //oCols = (oCols / sectors) * sectors;
    ////cl_int oRows = 1;
    //cl_int oRows = 1 + ((iRows - kernelDim) / stride);
    ////convPerSect = 3;
    //convPerSect = oCols / sectors;

    initializeKernels();
    OpenCLInfo* openCLInfo = OpenCLSetup(0, 0, kernels, NumberOfKernels);

    ConvolutionInfo* info = new ConvolutionInfo(iCols, iRows, kernelDim, stride, numChannels, sectors, openCLInfo);

    cv::Mat* in = new cv::Mat(info->iHeight, info->iWidth, CV_8UC3);
    cv::Mat* out = new cv::Mat(info->oHeight, info->oWidth, CV_8UC3);

    // Gdiplus Variables
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    SharedImage* sharedIn = new SharedImage(in, ImageType::RGB_24bpp, openCLInfo);

    SharedImage* sharedOut = new SharedImage(out, ImageType::RGB_24bpp, openCLInfo);

    int inputSize = info->iHeight * info->iWidth * info->numChannels;
    int kernelDatSize = info->kernelSize * info->kernelSize * info->numChannels;
    int outputSize = info->oHeight * info->oWidth * info->numChannels;

    int result = 0;

    OpenCLBuffer* input = &sharedIn->getOpenCLMatrix()->data;
    OpenCLBuffer* output = &sharedOut->getOpenCLMatrix()->data;
    OpenCLBuffer* kernelDat = &info->imgKernel->getKernelData()->data;

    //OpenCLBuffer* input = createOpenCLBufferHp(inputSize, sizeof(char), *openCLInfo, CL_MEM_READ_WRITE, &result);
    //OpenCLBuffer* kernelDat = createOpenCLBufferHp(kernelDatSize, sizeof(float), *openCLInfo, CL_MEM_READ_WRITE, &result);
    //OpenCLBuffer* output = createOpenCLBufferHp(outputSize, sizeof(char), *openCLInfo, CL_MEM_READ_WRITE, &result);

    //cl_kernel kernel = clCreateKernel(openCLInfo->program, nameOfConvolutionOperator, &result);

    //Profiler profiler = Profiler();

    //profiler.startProfile("setArg");
    //result |= clSetKernelArg(kernel, 0, sizeof(input->buffer), &input->buffer);
    //result |= clSetKernelArg(kernel, 1, sizeof(iCols), &iCols);
    //result |= clSetKernelArg(kernel, 2, sizeof(kernelDat->buffer), &kernelDat->buffer);
    //result |= clSetKernelArg(kernel, 3, sizeof(kernelDim), &kernelDim);
    //result |= clSetKernelArg(kernel, 4, sizeof(output->buffer), &output->buffer);
    //result |= clSetKernelArg(kernel, 5, sizeof(oCols), &oCols);
    //result |= clSetKernelArg(kernel, 6, sizeof(numChannels), &numChannels);
    //result |= clSetKernelArg(kernel, 7, sizeof(stride), &stride);
    //result |= clSetKernelArg(kernel, 8, sizeof(convPerSect), &convPerSect);
    //profiler.endProfile("setArg");

    //profiler.printAllMilliseconds();

    hostInput = (unsigned char*)input->hostData;
    for (int i = 0; i < inputSize; i += numChannels) {
        for (int c = 0; c < numChannels; c++) {
            hostInput[i + c] = c + 1;
        }
    }
    result |= writeData(*openCLInfo, input);

    int kV = 0;
    hostKernel = (float*)kernelDat->hostData;
    //for (int i = 0; i < kernelDatSize; i++) {
    //    if (i % (kernelDim * kernelDim) == 0) {
    //        kV++;
    //    }
    //    hostKernel[i] = kV;
    //}
    for (int i = 0; i < kernelDatSize; i++) {
        hostKernel[i] = (i + 1) * .01;
    }
    result |= writeData(*openCLInfo, kernelDat);

    std::cout << "Input Actual Format: \n";
    int iF = 0;
    for (int y = 0; y < iRows; y++) {
        for (int x = 0; x < iCols; x++) {
            for (int c = 0; c < numChannels; c++) {
                std::cout << std::to_string(hostInput[iF]) << " ";
                iF++;
            }
        }
        std::cout << "\n";
    }

    std::cout << "\nInput By Channel: \n";
    int iC = 0;
    for (int c = 0; c < numChannels; c++) {
        iC = c;
        for (int y = 0; y < iRows; y++) {
            for (int x = 0; x < iCols; x++) {
                std::cout << std::to_string(hostInput[iC]) << " ";
                iC += numChannels;
            }
            std::cout << "\n";
        }

        std::cout << "\n";
    }

    std::cout << "\nKernel by Channel: \n";
    iC = 0;
    for (int c = 0; c < numChannels; c++) {
        for (int y = 0; y < kernelDim; y++) {
            for (int x = 0; x < kernelDim; x++) {
                std::cout << std::to_string(hostKernel[iC]) << " ";
                iC++;
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    hostOutput = (unsigned char*)output->hostData;
    memset(hostOutput, 0, sizeof(char*) * outputSize);

//#define GPU
//#define CPU
#define UseOperator

#ifdef GPU
    size_t* workOffset = (size_t*)malloc(sizeof(size_t) * 2); workOffset[0] = 0; workOffset[1] = 0;
    size_t* globalWorKSize = (size_t*)malloc(sizeof(size_t) * 2); globalWorKSize[0] = oRows; globalWorKSize[1] = sectors;
    size_t* localWorKSize = (size_t*)malloc(sizeof(size_t) * 2); localWorKSize[0] = 1; localWorKSize[1] = 1;
    result |= clEnqueueNDRangeKernel(openCLInfo->commandQueue, kernel, 2, workOffset, globalWorKSize, localWorKSize, 0, 0, 0);

    result |= clFinish(openCLInfo->commandQueue);

    result |= readData(*openCLInfo, output);
#endif
#ifdef CPU
    ////ConvolutionOperator(hostInput, iCols, hostKernel, kernelDim, hostOutput, oCols, numChannels, stride, convPerSect);
    //int workDim = 2;
    //int* IDSizes = (int*)malloc(sizeof(int) * workDim);
    //IDSizes[0] = oRows;
    //IDSizes[1] = sectors;
    //testKernel(testConv, workDim, IDSizes);
#endif
#ifdef UseOperator

    ConvolutionOperator* op = new ConvolutionOperator(sharedIn, sharedOut, info, nameOfConvolutionOperator);

    op->start();

    Sleep(1000);
    clFinish(openCLInfo->commandQueue);

    result |= readData(*openCLInfo, output);
#endif

#define AllowError

    std::cout << "\nOutput Actual Format: \n";
    iF = 0;
    for (int y = 0; y < info->oHeight; y++) {
        for (int x = 0; x < info->oWidth; x++) {
            for (int c = 0; c < numChannels; c++) {
#ifdef AllowError
                std::cout << std::to_string(hostOutput[iF]) << " ";
#else
                std::cout << hostOutput[iF] << " ";
#endif
                iF++;
            }
}
        std::cout << "\n";
    }

    std::cout << "\nOutput By Channel: \n";
    iC = 0;
    for (int c = 0; c < numChannels; c++) {
        iC = c;
        for (int y = 0; y < info->oHeight; y++) {
            for (int x = 0; x < info->oWidth; x++) {
#ifdef AllowError
                std::cout << std::to_string(hostOutput[iC]) << " ";
#else
                std::cout << hostOutput[iC] << " ";
#endif
                iC += numChannels;
            }
            std::cout << "\n";
    }

        std::cout << "\n";
    }

}

void DisplayCaptureTest() {
    GraphicsEngine* graphics = new GraphicsEngine(new InputRegistry());

    cv::VideoCapture camera = cv::VideoCapture(0);
    camera.open(0);

    cv::Mat* mat = new cv::Mat();
    camera.read(*mat);

    SharedImage* img = new SharedImage(mat, OpenCVToImageType(CV_8UC3));
    //ImageDisplayManager* disp = new ImageDisplayManager(img->getImage(), NULL);

    Profiler profiler = Profiler();
    profiler.addProfile("Take Picture");

    //graphics->addPaintable(disp);
    graphics->createWindow();
    double numPics = 0;;

    MSG messages;
    while (GetMessage(&messages, NULL, 0, 0) > 0) {
        TranslateMessage(&messages);
        //std::cout << "Message Recieved\n";
        DispatchMessage(&messages);
        profiler.startProfile("Take Picture");
        camera >> *mat; // THIS CAUSES STUPID FRAME RATE
        profiler.endProfile("Take Picture");
        numPics++;
        printf("Observed FPS: %f\n", graphics->getObservedFPS());
    }

    profiler.printMilliseconds("Take Picture");
    printf("Total Images: %i\n", (int)numPics);
    printf("Per Image: %f\n", (profiler.getProfile("Take Picture")->get() / 1000000) / numPics);

}

void DisplayCaptureTest(SharedImage* img) {
    GraphicsEngine* graphics = new GraphicsEngine(new InputRegistry());

    ImageDisplay* disp = new ImageDisplay(img->getImage());

    Profiler profiler = Profiler();
    profiler.addProfile("Take Picture");

    graphics->addPaintable(disp);
    graphics->createWindow();
    double numPics = 0;;

    MSG messages;
    while (GetMessage(&messages, NULL, 0, 0) > 0) {
        TranslateMessage(&messages);
        //std::cout << "Message Recieved\n";
        DispatchMessage(&messages);
        profiler.startProfile("Take Picture");
        profiler.endProfile("Take Picture");
        numPics++;
        printf("Observed FPS: %f\n", graphics->getObservedFPS());
    }

    profiler.printMilliseconds("Take Picture");
    printf("Total Images: %i\n", (int)numPics);
    printf("Per Image: %f\n", (profiler.getProfile("Take Picture")->get() / 1000000) / numPics);

}

void testBlur() {
    int result;

    cv::Mat* mat = new cv::Mat();
    cv::VideoCapture* camera = new cv::VideoCapture(0);
    camera->open(0);

    camera->read(*mat);

    delete camera;
    cv::imwrite("Original.jpg", *mat);

    initializeKernels();
    OpenCLInfo* openCLInfo = OpenCLSetup(0, 0, kernels, NumberOfKernels);

    cl_int iCols = mat->cols;
    cl_int iRows = mat->rows;
    cl_int kernelDim = 7;
    cl_int stride = 1;
    cl_int sectors = 10;
    cl_int numChannels = 3;

    ConvolutionInfo* convInfo = new ConvolutionInfo(iCols, iRows, kernelDim, stride, numChannels, sectors, openCLInfo);

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    SharedImage* input = new SharedImage(mat, ImageType::RGB_24bpp, openCLInfo);
    writeData(*openCLInfo, &input->getOpenCLMatrix()->data);

    SharedImage* output = new SharedImage(convInfo->oWidth, convInfo->oHeight, ImageType::RGB_24bpp, openCLInfo);

    OpenCLBuffer* kernelDat = &convInfo->imgKernel->getKernelData()->data;
    float* hostKernel = (float*)kernelDat->hostData;
    for (int i = 0; i < kernelDat->bufferLength; i++) {
        hostKernel[i] = 1.0 / (kernelDim * kernelDim);
    }
    writeData(*openCLInfo, kernelDat);

    cl_kernel kernel = clCreateKernel(openCLInfo->program, nameOfConvolutionOperator, &result);

    ConvolutionOperator* op = new ConvolutionOperator(input, output, convInfo, kernel);

    Profiler profiler = Profiler();
    profiler.startProfile("op");
    op->start();

    std::cout << "And Now We Wait\n";
    volatile int cat = 0;
    while (!op->isFinished()) {
        cat++;
        //Sleep(1000);
        //std::cout << cat << std::endl;
    }
    profiler.endProfile("op");
    profiler.printAllMilliseconds();
    //Sleep(3000);

    readData(*openCLInfo, &output->getOpenCLMatrix()->data);

    cv::Mat* out = output->getMat();
    ////cv::Mat* newOut = new cv::Mat(out->rows, out->cols, CV_8UC3);
    //for (int r = 0; r < out->rows; r++) {
    //    for (int c = 0; c < out->cols; c++) {
    //        for (int i = 0; i < mat->channels(); i++) {
    //            int indexO = (r * out->cols + c) * numChannels + i;
    //            int indexN = (r * newOut->cols + c) * numChannels + i;
    //            //std::cout << std::to_string(mat->data[index]) << " ";
    //            newOut->data[indexN] = out->data[indexO];
    //        }
    //        //std::cout << "  ";
    //    }
    //    //std::cout << "\n";
    //}

    //DisplayCaptureTest(output);

    bool writeResult = cv::imwrite("New.jpg", *out);
    std::cout << "Result of Write: " << writeResult << "\n";
}

void runProgram() {
    int result = 0;

    cv::Mat* mat = new cv::Mat();
    cv::VideoCapture* camera = new cv::VideoCapture(0);
    camera->open(0);
    camera->read(*mat);

    initializeKernels();
    OpenCLInfo* openCLInfo = OpenCLSetup(0, 0, kernels, NumberOfKernels);

    cl_ulong size;
    clGetDeviceInfo(openCLInfo->device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &size, 0);
    std::cout << "\n\n\n\nLocal Mem Size: " << size << "\n\n\n";

    cl_int iCols = mat->cols;
    cl_int iRows = mat->rows;
    cl_int kernelDim = 21;
    cl_int stride = 1;
    cl_int sectors = 100;
    cl_int numChannels = 3;

    ConvolutionInfo* convInfo = new ConvolutionInfo(iCols, iRows, kernelDim, stride, numChannels, sectors, openCLInfo);

    InputRegistry* registry = new InputRegistry();
    GraphicsEngine* graphicsEngine = new GraphicsEngine(registry);

    OpenCLBuffer* kernelDat = &convInfo->imgKernel->getKernelData()->data;
    float* hostKernel = (float*)kernelDat->hostData;

    float* rData = hostKernel;
    float* gData = rData + kernelDim * kernelDim;
    float* bData = gData + kernelDim * kernelDim;

    int center = (kernelDim * kernelDim - 1) * .5;

    std::vector<float> weights = std::vector<float>();
    for (int i = 0; i < (kernelDim + 1) / 2; i++) {
        //int sideLength = kernelDim - 2 * i;
        //int perim = 2 * sideLength + 2 * (sideLength - 2);
        //if (perim == 0) {
        //    perim = 1;
        //}
        weights.push_back(kernelDim / 2 - i);
    }

    //for (int i = 0; i < weights.size(); i++) {
    //    std::cout << weights[i] << " ";
    //}
    //std::cout << std::endl;

    Kernel* kernel = convInfo->imgKernel;
    kernel->setCenter(0, 1);
    kernel->setRingWeightsNormalized(1, weights);
    kernel->setCenter(2, 1);

    //kernel->setCenter(0, 0); // B
    //kernel->setCenter(1, 1); // G
    //kernel->setCenter(2, 1); // R

    //kernel->setAverageFilter(0);
    //kernel->setCenter(1, 1);
    //kernel->setCenter(2, 1);


    kernel->printKernel();
    //int i = 0;
    //for (int c = 0; c < numChannels; c++) {
    //    for (int row = 0; row < kernelDim; row++) {
    //        for (int col = 0; col < kernelDim; col++) {
    //            std::cout << std::to_string(hostKernel[i]) << " ";
    //            i++;
    //        }
    //        std::cout << "\n";
    //    }
    //    std::cout << "\n\n";
    //}

    writeData(*openCLInfo, kernelDat);

    //cl_kernel kernel = clCreateKernel(openCLInfo->program, nameOfConvolutionOperator, &result);

    std::vector<Operator*> operators = std::vector<Operator*>();
    int numOperators = 5;
    for (int i = 0; i < numOperators; i++) {
        SharedImage* input = new SharedImage(convInfo->iWidth, convInfo->iHeight, ImageType::RGB_24bpp, openCLInfo);
        SharedImage* output = new SharedImage(convInfo->oWidth, convInfo->oHeight, ImageType::RGB_24bpp, openCLInfo);
        Operator* op = new ConvolutionOperator(input, output, convInfo, nameOfConvolutionOperator);
        operators.push_back(op);
    }

    ImageSource* camSource = new OCVVideoCapture(camera, 0, true);
    //cv::String str = cv::String("C:\\Users\\eddie\\Downloads\\Programming\\C++ Programs\\LiveKernelConvolution\\LiveKernelConvolution\\InputImage.png");
    //cv::Mat fileImg = cv::imread(str);
    //ImageSource* fileSource = new FileImageSource(&fileImg);
    ImageDisplayManager* dispManager = new ImageDisplayManager(camSource, operators);

    graphicsEngine->addPaintable(dispManager);

    graphicsEngine->createWindow();
    dispManager->startHandleThreads();

    MSG messages;
    while (GetMessage(&messages, NULL, 0, 0) > 0) {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

}