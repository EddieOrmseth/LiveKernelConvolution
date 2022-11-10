#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <unordered_map>

class Profiler {

public:

	class Profile {

	public:

		Profile(std::string name);

		void start();
		void end();

		std::chrono::nanoseconds get();

		std::string getName();

		std::string toStringNanoseconds();
		std::string toStringMicroseconds();
		std::string toStringMilliseconds();
		std::string toStringSeconds();

		void printNanoseconds();
		void printMicroseconds();
		void printMilliseconds();
		void printSeconds();

	private:

		std::string name;

		bool running;
		std::chrono::high_resolution_clock::time_point startTime;

		std::chrono::nanoseconds totalTime;

	};

	Profiler();
	~Profiler();

	void addProfile(const char* name);
	void addProfile(std::string name);
	void addProfile(Profile* profile);

	void startProfile(const char* name);
	void startProfile(std::string name);

	void endProfile(const char* name);
	void endProfile(std::string name);

	Profile* getProfile(const char* name);
	Profile* getProfile(std::string name);

	std::chrono::nanoseconds getTotalTimeFromProfile(const char* name);
	std::chrono::nanoseconds getTotalTimeFromProfile(std::string name);

	std::string toStringNanoseconds(const char* name);
	std::string toStringNanoseconds(std::string name);
	std::string toStringMicroseconds(const char* name);
	std::string toStringMicroseconds(std::string name);	
	std::string toStringMilliseconds(const char* name);
	std::string toStringMilliseconds(std::string name);
	std::string toStringSeconds(const char* name);
	std::string toStringSeconds(std::string name);

	void printNanoseconds(const char* name);
	void printNanoseconds(std::string name);
	void printMicroseconds(const char* name);
	void printMicroseconds(std::string name);
	void printMilliseconds(const char* name);
	void printMilliseconds(std::string name);
	void printSeconds(const char* name);
	void printSeconds(std::string name);

	std::string allToStringNanoseconds();
	std::string allToStringMicroseconds();
	std::string allToStringMilliseconds();
	std::string allToStringSeconds();

	void printAllNanoseconds();
	void printAllMicroseconds();
	void printAllMilliseconds();
	void printAllSeconds();

private:

	std::unordered_map<std::string, Profile*>* profiles;

};
