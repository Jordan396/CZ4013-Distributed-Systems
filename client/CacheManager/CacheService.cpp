#include "CacheService.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include "../Global.h"

namespace fs = std::experimental::filesystem;
using namespace std;


bool CacheService::cacheFile(std::string pathName, char* text, std::chrono::system_clock::time_point time)
{
	// convert to current directory cache file name
	string cachepath = "../Client/CacheManager/TempFiles/" + extractFileName(pathName);

	// update hashing map
	if (cacheMap.find(pathName) == cacheMap.end()) {
		// does not exist in map
		File file(cachepath, time);
		cacheMap.insert({ pathName, file });
	}
	else {
		// exist in map
		File file(cachepath, time);
		cacheMap.at(pathName) = file;
	}

	// writing to the file in temp folder
	if (!writeFile(cachepath, text)) {
		return false;
	}
	else {
		return true;
	}
}

bool CacheService::clearFile(std::string pathName)
{
	// convert to current directory cache file name
	string cachepath = "../Client/CacheManager/TempFiles/" + extractFileName(pathName);

	try {
		// first try removing, if removing fails then cacheMap will not have the key as well
		fs::remove(cachepath);
		// update hashing map
		cacheMap.erase(pathName);
		return true;
	}
	catch (const fs::filesystem_error & e) {
		cout << e.what() << endl;
		return false;
	}
}

bool CacheService::clearCache()
{
	try {
		// remove all files in directory recursively
		fs::remove_all("../Client/CacheManager/TempFiles/");
		// update hashing map
		cacheMap.clear();
		return true;
	}
	catch (const fs::filesystem_error & e) {
		cout << e.what() << endl;
		return false;
	}
}

string CacheService::getCachedFile(std::string pathName)
{
	// convert to current directory cache file name
	string cachepath = "../Client/CacheManager/TempFiles/" + extractFileName(pathName);

	map<string, File>::iterator it;
	it = cacheMap.find(pathName);

	if (it == cacheMap.end()) {
		// does not exist in map
		return "";
	}
	else {
		// exist in map
		File file = it->second;
		if (file.computeTimeElapsed() > freshnessInterval) {
			// check if exceed the freshness interval 
			cout << "exceed the time limit by " << file.computeTimeElapsed() << endl;
			// TODO: send a msg to server to ask for the lastModified Date
			// if received lastModified Date not the same as current createdDate then return null else set fr
			//return NULL;
		}

		// read file and send back from cache 
		return readFile(cachepath);
	}
}

CacheService::~CacheService()
{
	for (map<int, X*>::iterator it = cacheMap.begin(); it != cacheMap.end(); ++it)
		delete it->second;
}

string CacheService::extractFileName(string pathName)
{
	// to be configured depending on how user keys in
	const size_t last_slash_idx = pathName.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		pathName.erase(0, last_slash_idx + 1);
	}
	return pathName;
}

bool CacheService::writeFile(std::string pathName, char* text)
{
	ofstream fp;
	fp.open(pathName);
	// Check for file object (detecting the stream state)
	if (!fp) {
		fs::create_directory("../Client/CacheManager/TempFiles");
		fp.open(pathName);
	}

	// write text into file 
	while (*text != '\0') {
		fp << *text;
		text++;
	}
	fp.flush();
	fp.close();
	return true;
}

string CacheService::readFile(std::string pathName)
{
	ifstream in(pathName);

	string contents((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

	in.close();

	return contents;
}


