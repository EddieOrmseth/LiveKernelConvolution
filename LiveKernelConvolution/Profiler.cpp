#include "Profiler.hpp"

Profiler::Profile::Profile(std::string name) {
	this->name = name;
	running = false;
	totalTime = std::chrono::nanoseconds(0);
}

void Profiler::Profile::start() {
	if (running) {
		return;
	}
	else {
		running = true;
		startTime = std::chrono::high_resolution_clock::now();
	}
}

void Profiler::Profile::end() {
	if (!running) {
		return;
	}
	else {
		running = false;
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds delta = endTime - startTime;
		totalTime += delta;
	}
}

std::chrono::nanoseconds Profiler::Profile::get() {
	if (running) {
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds delta = endTime - startTime;
		return totalTime + delta;
	}
	else {
		return totalTime;
	}
}

std::string Profiler::Profile::Profile::getName() {
	return name;
}

std::string Profiler::Profile::toStringNanoseconds() {
	return std::string(name + ": " + std::to_string(get().count()) + " nanoseconds");
}

std::string Profiler::Profile::toStringMicroseconds() {
	return std::string(name + ": " + std::to_string(get().count() / 1000) + " microseconds");
}

std::string Profiler::Profile::toStringMilliseconds() {
	return std::string(name + ": " + std::to_string(get().count() / 1000000) + " milliseconds");
}

std::string Profiler::Profile::toStringSeconds() {
	return std::string(name + ": " + std::to_string(get().count() / 1000000000) + " seconds");
}

void Profiler::Profile::printNanoseconds() {
	std::cout << toStringNanoseconds() << std::endl;
}

void Profiler::Profile::printMicroseconds() {
	std::cout << toStringMicroseconds() << std::endl;
}

void Profiler::Profile::printMilliseconds() {
	std::cout << toStringMilliseconds() << std::endl;
}

void Profiler::Profile::printSeconds() {
	std::cout << toStringSeconds() << std::endl;
}

Profiler::Profiler() {
	profiles = new std::unordered_map<std::string, Profile*>();
}

Profiler::~Profiler() {
	delete profiles;
}

void Profiler::addProfile(const char* name) {
	addProfile(std::string(name));
}

void Profiler::addProfile(std::string name) {
	addProfile(new Profile(name));
}

void Profiler::addProfile(Profile* profile) {
	profiles->emplace(profile->getName(), profile);
}

void Profiler::startProfile(const char* name) {
	startProfile(std::string(name));
}

void Profiler::startProfile(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		profile = new Profile(name);
		addProfile(profile);
	}
	profile->start();
}

void Profiler::endProfile(const char* name) {
	endProfile(std::string(name));
}

void Profiler::endProfile(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot End a Profile that does not exist!\n";
	}
	profile->end();
}

Profiler::Profile* Profiler::getProfile(const char* name) {
	return getProfile(std::string(name));
}

Profiler::Profile* Profiler::getProfile(std::string name) {
	if (profiles->find(name) == profiles->end()) {
		return nullptr;
	}
	else {
		return profiles->at(name);;
	}
}

std::chrono::nanoseconds Profiler::getTotalTimeFromProfile(const char* name) {
	return getTotalTimeFromProfile(std::string(name));
}

std::chrono::nanoseconds Profiler::getTotalTimeFromProfile(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot get a Profile that does not exist!\n";
	}
	return profile->get();
}

std::string Profiler::toStringNanoseconds(const char* name) {
	return toStringNanoseconds(std::string(name));
}

std::string Profiler::toStringNanoseconds(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot stringify a Profile that does not exist!\n";
	}
	return profile->toStringNanoseconds();
}

std::string Profiler::toStringMicroseconds(const char* name) {
	return toStringMicroseconds(std::string(name));
}

std::string Profiler::toStringMicroseconds(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot stringify a Profile that does not exist!\n";
	}
	return profile->toStringMicroseconds();
}

std::string Profiler::toStringMilliseconds(const char* name) {
	return toStringMilliseconds(std::string(name));
}

std::string Profiler::toStringMilliseconds(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot stringify a Profile that does not exist!\n";
	}
	return profile->toStringMilliseconds();
}

std::string Profiler::toStringSeconds(const char* name) {
	return toStringSeconds(std::string(name));
}

std::string Profiler::toStringSeconds(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot stringify a Profile that does not exist!\n";
	}
	return profile->toStringSeconds();
}

void Profiler::printNanoseconds(const char* name) {
	printNanoseconds(std::string(name));
}

void Profiler::printNanoseconds(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot print a Profile that does not exist!\n";
	}
	profile->printNanoseconds();
}

void Profiler::printMicroseconds(const char* name) {
	printMicroseconds(std::string(name));
}

void Profiler::printMicroseconds(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot print a Profile that does not exist!\n";
	}
	profile->printMicroseconds();
}

void Profiler::printMilliseconds(const char* name) {
	printMilliseconds(std::string(name));
}

void Profiler::printMilliseconds(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot print a Profile that does not exist!\n";
	}
	profile->printMilliseconds();
}

void Profiler::printSeconds(const char* name) {
	printSeconds(std::string(name));
}

void Profiler::printSeconds(std::string name) {
	Profile* profile = getProfile(name);
	if (profile == nullptr) {
		throw "Cannot print a Profile that does not exist!\n";
	}
	profile->printSeconds();
}

std::string Profiler::allToStringNanoseconds() {
	std::string string = std::string();
	for (auto entry : *profiles) {
		string += entry.second->toStringNanoseconds() + "\n";
	}
	return string;
}

std::string Profiler::allToStringMicroseconds() {
	std::string string = std::string();
	for (auto entry : *profiles) {
		string += entry.second->toStringMicroseconds() + "\n";
	}
	return string;
}

std::string Profiler::allToStringMilliseconds() {
	std::string string = std::string();
	for (auto entry : *profiles) {
		string += entry.second->toStringMilliseconds() + "\n";
	}
	return string;
}

std::string Profiler::allToStringSeconds() {
	std::string string = std::string();
	for (auto entry : *profiles) {
		string += entry.second->toStringSeconds() + "\n";
	}
	return string;
}

void Profiler::printAllNanoseconds() {
	std::cout << allToStringNanoseconds();
}

void Profiler::printAllMicroseconds() {
	std::cout << allToStringMicroseconds();
}

void Profiler::printAllMilliseconds() {
	std::cout << allToStringMilliseconds();
}

void Profiler::printAllSeconds() {
	std::cout << allToStringSeconds();
}
