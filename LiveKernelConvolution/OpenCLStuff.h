#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "CL\cl.h"

struct OpenCLInfo {
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_program program;
	cl_command_queue commandQueue;
};

OpenCLInfo* OpenCLSetup(int platformNum, int deviceNum, const char** kernels, int kernelCount);
OpenCLInfo* OpenCLSetup(int platformNum, int deviceNum, const char** kernels, int kernelCount, int* resultPtr);

OpenCLInfo* OpenCLSetupDeviceAndPlatorm(OpenCLInfo* openCLInfo, int platformNum, int deviceNum, int* resultPtr);
OpenCLInfo* OpenCLSetupContextProgramAndQueue(OpenCLInfo* openCLInfo, const cl_context_properties* contextProperties, const char** kernels, int kernelCount, const cl_queue_properties* commandQueueProperties, int* resultPtr);

int printOpenCLDevices();

int getNumPlatforms();
int getNumDevices(int platformNum);

const char* getErrorMessage(cl_int errorCode);
