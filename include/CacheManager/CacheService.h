#pragma once
#include <string>
#include <map>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include "File.h"
#include "./../FileHandler.h"
#include "./../RFAcli.h"
#include "./../Global.h"
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/serialization/map.hpp>

class CacheService
{
public:
	// this method will be called when client wants to write a file 
	bool writeFile(std::string pathName, std::string text, int offset);

	// this method will be called when client wants to read a file 
	std::string readFile(std::string pathName, int offset, int bytes);

	// this method will be called when client wants to delete a particular cache entry
	bool clearFile(std::string pathName);

	// thid method will be called when client wants to clear the content of the file
	bool clearContent(std::string pathName);

	bool downloadFile(std::string remotePath, std::string cachePath);

	// this method will be called when client wants to clear the whole cache
	bool clearCache();

	// this method will list all the entries in the cache 
	std::vector<std::string> listCache();

	// this method will fetch the file to the cache (checking if it is in the cache and whether it is valid first)
	bool checkValidityFetch(std::string pathName);

	bool saveHashMap();
	bool restoreHashMap();

	CacheService();
	~CacheService();
private:
	FileHandler fh;
	RFAcli client;
	std::map<std::string, File> cacheMap;

	// write the file to the cache and update the server as well
	bool write(std::string pathName, char* text, int offset);

	// write the file from server to the cache whole
	bool writeAll(std::string pathName);

	// read the file from cache 
	std::string read(std::string pathName, int offset, int bytes);

	// this method will call writeAll and also update the cacheMap to include the time of creation of file
	bool fetchFile(std::string pathName);

	// for resolving file path
	std::string getLocalPathToFile(std::string fileName);
	std::string extractFileName(std::string remoteFilePath);
	std::string getAbsoluteFilePathToMainFolder();

	// Chong Yan comment
	void updateCacheMap(std::string pathName, chrono::system_clock::time_point time);

};

