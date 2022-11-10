#pragma once
#include <vector>
#include <string>
#include "OpenCLData.h"

class Kernel {

public:

	Kernel(int kernelSize, int numChannels, OpenCLInfo* openCLInfo);
	void writeKernel();

	void setAverageFilter(int channel);

	void setEntireFilter(int channel, float value);
	void setCenter(int channel, float value);

	void setRingValues(int channel, std::vector<float> weights);
	void setRingWeights(int channel, std::vector<float> weights);
	void setRingWeightsNormalized(int channel, std::vector<float> weights);

	float* getHostData();
	OpenCLMatrix* getKernelData();
	float* getChannelData(int channel);
	int channelSize();
	int totalSize();

	std::string kernelToString();
	void printKernel();

	std::string channelToString(int channel);
	void printChannel(int channel);

private:

	int kernelSize;
	int kernelSizeSqrd;
	int numChannels;

	OpenCLInfo* openCLInfo;

	OpenCLMatrix* kernelData;
	float* hostData;

};
