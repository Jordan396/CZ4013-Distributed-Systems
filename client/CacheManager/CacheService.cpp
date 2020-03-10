#include "CacheService.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include "../Global.h"
#include "../RFAcli.h"

namespace fs = std::experimental::filesystem;
using namespace std;


bool CacheService::clearFile(std::string pathName)
{
	try {
		// first try removing, if removing fails then cacheMap will not have the key as well
		fs::remove(getLocalPathToFile(pathName);
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
		fs::remove_all("../client/CacheManager/TempFiles/");
		// update hashing map
		cacheMap.clear();
		return true;
	}
	catch (const fs::filesystem_error & e) {
		cout << e.what() << endl;
		return false;
	}
}

vector<std::string> CacheService::listCache()
{
	vector<string> v;
	for (map<string, File>::iterator it = cacheMap.begin(); it != cacheMap.end(); ++it) {
		v.push_back(it->first);
	}
	return v;
}


CacheService::~CacheService()
{
	for (map<String, String>::iterator it = cacheMap.begin(); it != cacheMap.end(); ++it)
		delete it->second;
}

// this method will just write to the cache file
bool CacheService::write(std::string pathName, char* text, int offset)
{
	FILE* pFile;
	pFile = fopen(getLocalPathToFile(pathName))
	// Check for file object (detecting the stream state)
	if (!fp) {
		fs::create_directory("../client/CacheManager/TempFiles");
		fp.open(pathName);
	}

	// TODO (Chin to provide me the method)

}

// this method will call the server and transfer text chunk by chunk to the cache file 
bool CacheService::writeAll(std::string pathName)
{
	// TODO (include Jordan's method)
	return true;
}

std::string CacheService::read(std::string pathName, int offset, int bytes)
{
	// TODO (Chin to provide me the method) (added my previous code, pls replace if needed)
	FILE* pFile;
	char* buffer;
	size_t result;

	pFile = fopen(getLocalPathToFile(pathName), "rb");
	if (pFile == NULL) { return ""; }

	// obtain file size:
	fseek(pFile, offset, SEEK_SET);
	

	// allocate memory to contain the whole file:
	buffer = (char*)malloc(sizeof(char) * bytes);

	// copy the file into the buffer:
	fread(buffer, 1, bytes, pFile);

	string s(buffer);

	// terminate
	fclose(pFile);
	free(buffer);

	return s;
}

bool CacheService::writeFile(std::string pathName, char* text, int offset)
{
	if (checkValidityFetch(pathName)) {
		// now perform write to the cache file 
		if (write(getLocalPathToFile(pathName), text, offset)) {
			// inform the server about the change 
			RFACli rc();
			rc.write_file(pathName, text, offset);
			return true;
		}
	}

	return false;
}

std::string CacheService::readFile(std::string pathName, int offset, int bytes)
{
	if (checkValidityFetch(pathName)) {
		return read(pathName, offset, bytes);
	}
	else {
		return "File does not exist in current directory";
	}
}

// make sure the file requested is placed in cache
bool CacheService::checkValidityFetch(std::string pathName)
{
	map<string, File>::iterator it;
	it = cacheMap.find(pathName);

	if (it == cacheMap.end()) {
		// does not exist in map (thus need to request file from server again before proceeding)
		return fetchFile(pathName);
	}
	else {
		// exist in map (need to check freshness)
		File file = it->second;
		// check if exceed the freshness interval 
		// if does not exceed, then is valid
		if (file.computeTimeElapsed() <= freshnessInterval) {
			return true;
		}
		else {
			// check if server side last modified is the same 
			RFACli rc();
			//TODO (Server side to provide method)
			if (rc.getLastModifiedTime(pathName) != file.createdTime) {
				return fetchFile(pathName);
			}
			else {
				return true;
			}
		}
	}

}

bool CacheService::fetchFile(std::string pathName)
{
	// convert to current directory cache file name
	string cachepath = getLocalPathToFile(pathName);

	RFACli rc();
	// get the modified date from server 
	chrono::system_clock::time_point time = rc.getTime(pathName);

	// write from the server to the cache 
	if (writeAll(pathName)) {
		// update the hashing table 
		updateCacheMap(pathName, time);
		return true;
	}
	else {
		return false;
	}
}

void CacheService::updateCacheMap(std::string pathName, chrono::system_clock::time_point time) {
	if (cacheMap.find(pathName) == cacheMap.end()) {
		// does not exist in map
		File file(getLocalPathToFile(pathName), time);
		cacheMap.insert({ pathName, file });
	}
	else {
		// exist in map
		File file(getLocalPathToFile(pathName), time);
		cacheMap.at(pathName) = file;
	}
}


// For file name parsing
string CacheService::extractFileName(string remoteFilePath)
{
	// to be configured depending on how user keys in
	const size_t last_slash_idx = remoteFilePath.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		remoteFilePath.erase(0, last_slash_idx + 1);
	}
	return remoteFilePath;
}

bool CacheService::saveHashMap()
{
	if (cacheMap.empty())
		return false;

	FILE* fp = fopen("../client/CacheManager/TempFiles/cacheHistory.txt", "w");
	if (!fp) {
		fs::create_directory("../client/CacheManager/TempFiles");
		fp = fopen("../client/CacheManager/TempFiles/cacheHistory.txt", "w");
	}

	for (map<string, File>::iterator it = cacheMap.begin(); it != cacheMap.end(); it++) {
		fprintf(fp, "%s=%s\n", it->first.c_str(), it->second.c_str());
	}

	fclose(fp);
	return true;
}

bool CacheService::restoreHashMap()
{
	FILE* fp = fopen("../client/CacheManager/TempFiles/cacheHistory.txt", "r");
	if (!fp) return false;

	cacheMap.clear();

	char* buf = 0;
	size_t buflen = 0;

	while (getline(&buf, &buflen, fp) > 0) {
		char* nl = strchr(buf, '\n');
		if (nl == NULL)
			continue;
		*nl = 0;

		char* sep = strchr(buf, '=');
		if (sep == NULL)
			continue;
		*sep = 0;
		sep++;

		string s1 = buf;
		string s2 = sep;

		cacheMap[s1] = s2;
	}

	if (buf)
		free(buf);

	fclose(fp);
	return true;
}

std::string CacheService::getLocalPathToFile(std::string fileName)
{
	return "../client/CacheManager/TempFiles/" + extractFileName(fileName);
}




