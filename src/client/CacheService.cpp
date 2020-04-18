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

bool CacheService::clearFile(std::string pathName)
{
  try
  {
    // first try removing, if removing fails then cacheMap will not have the key as well
    fs::remove(getLocalPathToFile(pathName));
    // update hashmap
    cacheMap.erase(pathName);
    return true;
  }
  catch (const fs::filesystem_error &e)
  {
    cout << e.what() << endl;
    return false;
  }
}

bool CacheService::monitorFile(std::string pathName, int monitorDuration)
{
  int res = client.register_client(pathName, getLocalPathToFile(pathName), to_string(monitorDuration));
  return res == 1;
}

bool CacheService::clearCache()
{
  try
  {
    // remove all files in directory recursively
    string directoryPath = getAbsoluteFilePathToMainFolder() + "/ClientCache";
    fs::remove_all(directoryPath);
    // update hashing map
    cacheMap.clear();
    fs::create_directory(directoryPath);
    return true;
  }
  catch (const fs::filesystem_error &e)
  {
    cout << e.what() << endl;
    return false;
  }
}

vector<std::string> CacheService::listCache()
{
  vector<string> v;
  for (map<string, File>::iterator it = cacheMap.begin(); it != cacheMap.end();
       ++it)
  {
    v.push_back(it->first);
  }
  return v;
}

CacheService::~CacheService()
{
  for (map<string, File>::iterator it = cacheMap.begin(); it != cacheMap.end();
       ++it)
    it = cacheMap.erase(it);
}

/**
 * @brief clear the content in the specified file
 * Method will update both the local and remote copies of the file
 * @param pathName remote pathName
 * @return true if success, false if failure
 */
bool CacheService::clearContent(std::string pathName)
{
  // does not have to do validity check as is simply clear file content

  if (fh.ClearFile(getLocalPathToFile(pathName).c_str()) == 1)
  {
    client.clear_file(pathName);
    return true;
  }
  return false;
}

bool CacheService::downloadFile(std::string remotePath, std::string cachePath)
{
  int result = client.download_file(remotePath, cachePath);
  return result == 1;
}

bool CacheService::writeFile(std::string pathName, std::string text,
                             int offset)
{
  if (checkValidityFetch(pathName))
  {
    // now perform write to the cache file
    if (fh.WriteFile(getLocalPathToFile(pathName).c_str(), text.c_str(),
                     offset))
    {
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

std::string CacheService::readFile(std::string pathName, int offset,
                                   int bytes)
{
  char echoBuffer[bufferSize];
  if (checkValidityFetch(pathName))
  {
    fh.ReadFile(getLocalPathToFile(pathName).c_str(), echoBuffer, bytes,
                offset);
    string s(echoBuffer);
    return s;
  }
  else
  {
    return "File does not exist in current directory";
  }
}

bool CacheService::checkValidityFetch(std::string pathName)
{
  map<string, File>::iterator it;
  it = cacheMap.find(pathName);

  if (it == cacheMap.end())
  {
    // does not exist in map (thus need to request file from server again before
    // proceeding)
    cout << "file not exist" << endl;
    return fetchFile(pathName);
  }
  else
  {
    // exist in map (need to check freshness)
    cout << "file exist" << endl;
    File file = it->second;
    // check if exceed the freshness interval
    // if does not exceed, then is valid
    if (file.computeTimeElapsed() <= freshnessInterval)
    {
      return true;
    }
    else
    {
      // check if server side last modified is the same
      time_t last_modified_time = client.fetch_last_modified_time(pathName);

      if (difftime(last_modified_time, file.createdTime) != 0)
      {
        return fetchFile(pathName);
      }
      else
      {
        return true;
      }
    }
  }
}

void CacheService::updateCacheMap(std::string pathName, time_t time)
{
  if (cacheMap.find(pathName) == cacheMap.end())
  {
    // does not exist in map
    File file(getLocalPathToFile(pathName), time);
    cacheMap.insert({pathName, file});
  }
  else
  {
    // exist in map
    File file(getLocalPathToFile(pathName), time);
    cacheMap.at(pathName) = file;
  }
}

bool CacheService::fetchFile(std::string pathName)
{
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
  if (downloadFile(pathName, cachepath))
  {
    // update the hashing table
    updateCacheMap(pathName, last_modified_time);
    return true;
  }
  else
  {
    return false;
  }
}

string CacheService::extractFileName(string remoteFilePath)
{
  // to be configured depending on how user keys in
  const size_t last_slash_idx = remoteFilePath.find_last_of("\\/");
  if (std::string::npos != last_slash_idx)
  {
    return remoteFilePath.substr(last_slash_idx + 1);
  }
}

std::string CacheService::getLocalPathToFile(std::string fileName)
{
  string localPath = getAbsoluteFilePathToMainFolder() + "/ClientCache/" +
                     extractFileName(fileName);
  cout << "Local path: " + localPath << endl;
  return localPath;
}

std::string CacheService::getAbsoluteFilePathToMainFolder()
{
  return fs::current_path();
}
