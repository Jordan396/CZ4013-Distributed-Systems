#pragma once
#include <string>
#include <chrono>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
class File
{
public:
	File(std::string lfp, std::chrono::system_clock::time_point ct);
	std::string localFilePath;
	std::chrono::system_clock::time_point createdTime;
	int computeTimeElapsed();
	~File();
};

