#include "CacheService.h"

namespace fs = std::experimental::filesystem;
using namespace std;

bool CacheService::clearFile(std::string pathName)
{
	try {
		// first try removing, if removing fails then cacheMap will not have the key as well
		fs::remove(getLocalPathToFile(pathName));
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
		string directoryPath = getAbsoluteFilePathToMainFolder() + "ClientCache";
		fs::remove_all(directoryPath);
		// update hashing map
		cacheMap.clear();
		fs::create_directory(directoryPath);
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
	for (map<string, File>::iterator it = cacheMap.begin(); it != cacheMap.end(); ++it)
		it = cacheMap.erase(it);
}

// // this method will just write to the cache file
// bool CacheService::write(std::string pathName, char* text, int offset)
// {
// 	string fileName = getLocalPathToFile(pathName)
	
// 	// guarantee of existence of file pointed to by pathName 
// 	fh.WriteFile(fileName.c_str(), text, )
// 	// FILE* pFile;
// 	// pFile = fopen(getLocalPathToFile(pathName).c_str(), );
// 	// Check for file object (detecting the stream state)

// 	// TODO: create somewhere else 
// 	// fs::create_directory("../client/CacheManager/TempFiles");
// 	if (!fp) {
// 		fp.open(pathName);
// 	}

// 	// TODO (Chin to provide me the method)

// }

// this method will call the server and transfer text chunk by chunk to the cache file 
bool CacheService::downloadFile(std::string remotePath, std::string cachePath)
{
	// TODO (include Jordan's method)
	int result = client.download_file(remotePath, cachePath);
	return result==1;
}

// std::string CacheService::read(std::string pathName, int offset, int bytes)
// {
// 	// TODO (Chin to provide me the method) (added my previous code, pls replace if needed)
// 	FILE* pFile;
// 	char* buffer;
// 	size_t result;

// 	pFile = fopen(getLocalPathToFile(pathName), "rb");
// 	if (pFile == NULL) { return ""; }

// 	// obtain file size:
// 	fseek(pFile, offset, SEEK_SET);
	

// 	// allocate memory to contain the whole file:
// 	buffer = (char*)malloc(sizeof(char) * bytes);

// 	// copy the file into the buffer:
// 	fread(buffer, 1, bytes, pFile);

// 	string s(buffer);

// 	// terminate
// 	fclose(pFile);
// 	free(buffer);

// 	return s;
// }

bool CacheService::writeFile(std::string pathName, std::string text, int offset)
{
	if (checkValidityFetch(pathName)) {
		// now perform write to the cache file 
		if (fh.WriteFile(getLocalPathToFile(pathName).c_str(), text.c_str(), offset)) {
			// inform the server about the change 
			client.write_file(pathName, text, offset);
			return true;
		}
	}

	return false;
}

std::string CacheService::readFile(std::string pathName, int offset, int bytes)
{
	char echoBuffer[bufferSize];
	if (checkValidityFetch(pathName)) {
		fh.ReadFile(pathName.c_str(), echoBuffer, offset, bytes);
		string s(echoBuffer);
		return s;
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
			// RFACli rc();
			//TODO (Server side to provide method)
			string last_modified_time;
			client.get_last_modified_time(pathName);

			// TODO: Convert last_modified_time to below format

			std::tm tm = {0};
			tm.tm_sec = 40;
			tm.tm_min = 38;
			tm.tm_hour = 12;
			tm.tm_mday = 10;
			tm.tm_mon = 9;
			tm.tm_year = 112;
			tm.tm_isdst = -1;
			// Convert std::tm to std::time_t (popular extension)
			std::time_t tt = timegm(&tm);
			// Convert std::time_t to std::chrono::system_clock::time_point
			std::chrono::system_clock::time_point time = std::chrono::system_clock::from_time_t(tt);

			if (time != file.createdTime) {
				return fetchFile(pathName);
			}
			else {
				return true;
			}
		}
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

bool CacheService::fetchFile(std::string pathName)
{
	// convert to current directory cache file name
	string cachepath = getLocalPathToFile(pathName);
	// RFACli rc();
	string last_modified_time;
	client.get_last_modified_time(pathName);

	// TODO: Convert last_modified_time to below format

	std::tm tm = {0};
    tm.tm_sec = 40;
    tm.tm_min = 38;
    tm.tm_hour = 12;
    tm.tm_mday = 10;
    tm.tm_mon = 9;
    tm.tm_year = 112;
    tm.tm_isdst = -1;
    // Convert std::tm to std::time_t (popular extension)
    std::time_t tt = timegm(&tm);
    // Convert std::time_t to std::chrono::system_clock::time_point
    std::chrono::system_clock::time_point time = std::chrono::system_clock::from_time_t(tt);

	// write from the server to the cache 
	if (downloadFile(pathName,cachepath)) {
		// update the hashing table 
		updateCacheMap(pathName, time);
		return true;
	}
	else {
		return false;
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
	string filePath = getAbsoluteFilePathToMainFolder() + "ClientCache/cacheHistory.txt";
	FILE* fp = fopen(filePath.c_str(), "w");

	for (map<string, File>::iterator it = cacheMap.begin(); it != cacheMap.end(); it++) {
		fprintf(fp, "%s=%s\n", it->first.c_str(), it->second);
	}

	fclose(fp);
	return true;
}

bool CacheService::restoreHashMap()
{
	string filePath = getAbsoluteFilePathToMainFolder() + "ClientCache/cacheHistory.txt";

	FILE* fp = fopen(filePath.c_str(), "r");
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

		// cacheMap[s1] = s2;
	}

	if (buf)
		free(buf);

	fclose(fp);
	return true;
}

std::string CacheService::getLocalPathToFile(std::string fileName)
{
	return getAbsoluteFilePathToMainFolder() + "ClientCache/"+extractFileName(fileName);
}

std::string CacheService::getAbsoluteFilePathToMainFolder()
{
	string absolutePathForWorkingDirectory = fs::current_path();
	const size_t last_slash_idx = absolutePathForWorkingDirectory.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		absolutePathForWorkingDirectory.erase(last_slash_idx + 1, absolutePathForWorkingDirectory.length());
	}
	return absolutePathForWorkingDirectory;
}




