/**
 * @file CacheService.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Implementation code for CacheService.cpp
 */

#include "CacheService.h"

namespace fs = std::experimental::filesystem;
using namespace std;

CacheService::CacheService() {}

// /**
//  * @brief clear the copy of file in local directory
//  * Method will remove the the file in the local directory and update the hashmap accordingly
//  * @param pathName remote pathName
//  * @return true if clear copy of file is successful, false if it fails
//  */
bool CacheService::clearFile(std::string pathName) {
  try {
    // first try removing, if removing fails then cacheMap will not have the key as well
    fs::remove(getLocalPathToFile(pathName));
    // update hashmap
    cacheMap.erase(pathName);
    return true;
  } catch (const fs::filesystem_error &e) {
    cout << e.what() << endl;
    return false;
  }
}

// /**
//  * @brief register the client with the server and listen for new update to the file
//  * @param pathName remote pathName
//  * @param monitorDuration the time period for monitoring
//  * @return true if success, false if failure
//  */
bool CacheService::monitorFile(std::string pathName, int monitorDuration)
{
    int res = client.register_client(pathName, getLocalPathToFile(pathName), to_string(monitorDuration));
    return res == 1;
}

// /**
//  * @brief clear all the files stored in the local cache directory
//  * @param void
//  * @return true if success, false if failure
//  */
bool CacheService::clearCache() {
  try {
    // remove all files in directory recursively
    string directoryPath = getAbsoluteFilePathToMainFolder() + "/ClientCache";
    fs::remove_all(directoryPath);
    // update hashing map
    cacheMap.clear();
    fs::create_directory(directoryPath);
    return true;
  } catch (const fs::filesystem_error &e) {
    cout << e.what() << endl;
    return false;
  }
}

// /**
//  * @brief list all the files in the local cache directory and recorded in the cache hashmap
//  * @param void
//  * @return vector<string> vector of the different types of file names
//  */
vector<std::string> CacheService::listCache() {
  vector<string> v;
  for (map<string, File>::iterator it = cacheMap.begin(); it != cacheMap.end();
       ++it) {
    v.push_back(it->first);
  }
  return v;
}

CacheService::~CacheService() {
  for (map<string, File>::iterator it = cacheMap.begin(); it != cacheMap.end();
       ++it)
    it = cacheMap.erase(it);
}


// /**
//  * @brief clear the content in the specified file
//  * Method will update both the local and remote copies of the file
//  * @param pathName remote pathName
//  * @return true if success, false if failure
//  */
bool CacheService::clearContent(std::string pathName) {
  // does not have to do validity check as is simply clear file content

  if (fh.ClearFile(getLocalPathToFile(pathName).c_str()) == 1) {
      client.clear_file(pathName);
      return true;
  }
  return false;
}

// /**
//  * @brief download the remote file into the local file directory
//  * Method will call RFAcli's download_file method 
//  * @param remotePath remote pathName
//  * @param cachePath local pathName
//  * @return true if success, false if failure
//  */
bool CacheService::downloadFile(std::string remotePath, std::string cachePath) {
  int result = client.download_file(remotePath, cachePath);
  return result == 1;
}

// /**
//  * @brief write the file
//  * Method will first update the file in the local file directory and then update the server of the new file copy
//  * @param pathName remote pathName
//  * @param text text to be written into the file
//  * @param offset the offset of the file at which the append should start
//  * @return true if success, false if failure
//  */
bool CacheService::writeFile(std::string pathName, std::string text,
                             int offset) {
  if (checkValidityFetch(pathName)) {
    // now perform write to the cache file
    if (fh.WriteFile(getLocalPathToFile(pathName).c_str(), text.c_str(),
                     offset)) {
      // inform the server about the change
      cout << "The text which will be written over is: " << text << endl;
      client.write_file(pathName, text.c_str(), offset);
      time_t servertime = client.fetch_last_modified_time(pathName);
      updateCacheMap(pathName, servertime);
      return true;
    }
  }
  return false;
}

// /**
//  * @brief read the file based on the offset and the number of bytes to be read
//  * @param pathName remote pathName
//  * @param offset the offset of the file where reading should start
//  * @param bytes the number of bytes from the offset to be read
//  * @return string of text which is read, or error message if the reading fails
//  */
std::string CacheService::readFile(std::string pathName, int offset,
                                   int bytes) {
  char echoBuffer[bufferSize];
  if (checkValidityFetch(pathName)) {
    fh.ReadFile(getLocalPathToFile(pathName).c_str(), echoBuffer, bytes,
                offset);
    string s(echoBuffer);
    return s;
  } else {
    return "File does not exist in current directory";
  }
}

// /**
//  * @brief check whether the file in the local directory is valid, if not fetch the file again from the server to ensure validity
//  * @param pathName remote pathName
//  * @return true if the validity check and its operations are successful, false if any of them fails
//  */
bool CacheService::checkValidityFetch(std::string pathName) {
  map<string, File>::iterator it;
  it = cacheMap.find(pathName);

  if (it == cacheMap.end()) {
    // does not exist in map (thus need to request file from server again before
    // proceeding)
    cout << "file not exist" << endl;
    return fetchFile(pathName);
  } else {
    // exist in map (need to check freshness)
    cout << "file exist" << endl;
    File file = it->second;
    // check if exceed the freshness interval
    // if does not exceed, then is valid
    if (file.computeTimeElapsed() <= freshnessInterval) {
      return true;
    } else {
      // check if server side last modified is the same
      time_t last_modified_time = client.fetch_last_modified_time(pathName);

      if (difftime(last_modified_time, file.createdTime) != 0) {
        return fetchFile(pathName);
      } else {
        return true;
      }
    }
  }
}

// /**
//  * @brief update the entry in the cache hashmap
//  * @param pathName remote pathName
//  * @param time last_modified_time of the file
//  * @return void
//  */
void CacheService::updateCacheMap(std::string pathName, time_t time) {
  if (cacheMap.find(pathName) == cacheMap.end()) {
    // does not exist in map
    File file(getLocalPathToFile(pathName), time);
    cacheMap.insert({pathName, file});
  } else {
    // exist in map
    File file(getLocalPathToFile(pathName), time);
    cacheMap.at(pathName) = file;
  }
}

// /**
//  * @brief fetch the file from the remote server to the local file directory and update the last_modified_time record in hashmap
//  * @param pathName remote pathName
//  * @return true if fetch succeed, false if fetch fails
//  */
bool CacheService::fetchFile(std::string pathName) {
  // convert to current directory cache file name
  string cachepath = getLocalPathToFile(pathName);

  time_t last_modified_time = client.fetch_last_modified_time(pathName);

  // Debugging
  char last_modified_time_string[80];
  strftime(last_modified_time_string, 20, "%Y-%m-%d %H:%M:%S",
           gmtime(&last_modified_time));
  string s = last_modified_time_string;
  cout << "Last modified time in CacheService.cpp fetchFile: " + s << endl;

  // write from the server to the cache
  if (downloadFile(pathName, cachepath)) {
    // update the hashing table
    updateCacheMap(pathName, last_modified_time);
    return true;
  } else {
    return false;
  }
}

// /**
//  * @brief extract the file name from the remote file path
//  * @param remoteFilePath remote pathName
//  * @return string of the file name
//  */
string CacheService::extractFileName(string remoteFilePath) {
  // to be configured depending on how user keys in
  const size_t last_slash_idx = remoteFilePath.find_last_of("\\/");
  if (std::string::npos != last_slash_idx) {
    return remoteFilePath.substr(last_slash_idx + 1);
  }
}

// /**
//  * @brief get the local file path 
//  * @param fileName remote pathName
//  * @return string of the local file path
//  */
std::string CacheService::getLocalPathToFile(std::string fileName) {
  string localPath = getAbsoluteFilePathToMainFolder() + "/ClientCache/" +
                     extractFileName(fileName);
  cout << "Local path: " + localPath << endl;
  return localPath;
}

// /**
//  * @brief get the absolute file path to main folder 
//  * @param void
//  * @return string of the absolute file path
//  */
std::string CacheService::getAbsoluteFilePathToMainFolder() {
  return fs::current_path();
}
