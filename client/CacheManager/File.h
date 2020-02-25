#pragma once
#include <string>
#include <chrono>
class File
{
public:
	File(std::string lfp, std::chrono::system_clock::time_point ct);
	std::string localFilePath;
	std::chrono::system_clock::time_point createdTime;
	int computeTimeElapsed();
	~File();
};

