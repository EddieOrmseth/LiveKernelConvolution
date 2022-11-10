#include "Kernel.hpp"
#include <iostream>

Kernel::Kernel(int kernelSize, int numChannels, OpenCLInfo* openCLInfo) {
	this->kernelSize = kernelSize;
	this->kernelSizeSqrd = kernelSize * kernelSize;
	this->numChannels = numChannels;
	this->openCLInfo = openCLInfo;
	int result = 0;
	kernelData = createOpenCLMatrixHp(kernelSize, kernelSize, numChannels * sizeof(float), *openCLInfo, CL_MEM_READ_WRITE, &result);
	hostData = (float*)kernelData->data.hostData;
	memset(hostData, 0, kernelData->data.bufferRawSize);
}

void Kernel::writeKernel() {
	writeData(*openCLInfo, &kernelData->data);
}

void Kernel::setAverageFilter(int channel) {
	setEntireFilter(channel, 1.0 / kernelSizeSqrd);
}

void Kernel::setEntireFilter(int channel, float value) {
	float* data = getChannelData(channel);
	for (int i = 0; i < kernelSizeSqrd; i++) {
		data[i] = value;
	}
}

void Kernel::setCenter(int channel, float value) {
	float* data = getChannelData(channel);
	data[kernelSizeSqrd / 2] = value;
}

void Kernel::setRingValues(int channel, std::vector<float> weights) {
	int numRings = (kernelSize + 1) / 2;
	if (weights.size() != numRings) {
		printf("Unable to set Ring Weights, Number of Rings: %i    Number of Weights: %i\n", numRings, weights.size());
		return;
	}

	float* data = getChannelData(channel);
	int index;
	for (int r = 0; r < numRings; r++) {
		int sideLength = kernelSize - 2 * r;
		float val = weights[r];
		for (int x = r; x < kernelSize - r; x++) {
			index = r * kernelSize + x;
			data[index] = val;
			data[index + (sideLength - 1) * kernelSize] = val;
		}
		for (int y = r + 1; y < kernelSize - (r + 1); y++) {
			index = r + y * kernelSize;
			data[index] = val;
			data[index + (sideLength - 1)] = val;
		}
	}

	setCenter(channel, weights[weights.size() - 1]);
}

void Kernel::setRingWeights(int channel, std::vector<float> weights) {
	int numRings = (kernelSize + 1) / 2;
	if (weights.size() != numRings) {
		printf("Unable to set Ring Weights, Number of Rings: %i    Number of Weights: %i\n", numRings, weights.size());
		return;
	}

	float* data = getChannelData(channel);
	int index;
	for (int r = 0; r < numRings; r++) {
		int sideLength = kernelSize - 2 * r;
		int perim = 2 * sideLength + 2 * (sideLength - 2);
		if (perim == 0) {
			perim = 1;
		}
		weights[r] /= perim;
	}

	setRingValues(channel, weights);
}

void Kernel::setRingWeightsNormalized(int channel, std::vector<float> weights) {
	float total = 0;
	for (int i = 0; i < weights.size(); i++) { // Sum
		total += weights[i];
	}
	if (total != 0) {
		for (int i = 0; i < weights.size(); i++) { // Normalize
			weights[i] /= total;
		}
	} else {
		printf("Unable to normalize weights, total is 0\n");
	}

	setRingWeights(channel, weights);
}

float* Kernel::getHostData() {
	return hostData;
}

OpenCLMatrix* Kernel::getKernelData() {
	return kernelData;
}

float* Kernel::getChannelData(int channel) {
	return hostData + kernelSizeSqrd * channel;
}

int Kernel::channelSize() {
	return kernelSizeSqrd;
}

int Kernel::totalSize() {
	return kernelSizeSqrd * numChannels;
}

std::string Kernel::kernelToString() {
	std::string str = std::string();

	for (int c = 0; c < numChannels; c++) {
		str += channelToString(c) + "\n";
	}

	return str;
}

void Kernel::printKernel() {
	std::cout << kernelToString() << std::endl;
}

std::string Kernel::channelToString(int channel) {
	std::string str = std::string();

	float* data = getChannelData(channel);
	int i = 0;
	for (int row = 0; row < kernelSize; row++) {
		for (int col = 0; col < kernelSize; col++) {
			str += std::to_string(data[i]) + " ";
			i++;
		}
		str += "\n";
	}

	return str;
}

void Kernel::printChannel(int channel) {
	std::cout << channelToString(channel) << std::endl;
}
