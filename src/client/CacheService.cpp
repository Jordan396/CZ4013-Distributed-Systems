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
  Debug::msg("Now removing the file from cache");
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
    Debug::msg("Now monitoring the file");
  int res = client.register_client(pathName, getLocalPathToFile(pathName), to_string(monitorDuration));

  if (res == 1) {
      // fetch the last modified time and update the hashmap
      time_t servertime = client.fetch_last_modified_time(pathName);
      updateCacheMap(pathName, servertime);
  }
  return res == 1;
}

bool CacheService::clearCache()
{
  try
  {
      Debug::msg("Now clearing the cache");
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


bool CacheService::clearContent(std::string pathName)
{
  // does not have to do validity check as is simply clear file content
    Debug::msg("Now clearing the file content");
  if (fh.ClearFile(getLocalPathToFile(pathName).c_str()) == 1)
  {
      Debug::msg("Local clear success");

    client.clear_file(pathName);
    time_t servertime = client.fetch_last_modified_time(pathName);
    updateCacheMap(pathName, servertime);

    return true;
  }
  Debug::msg("Local clear failure");

  return false;
}

bool CacheService::downloadFile(std::string remotePath, std::string cachePath)
{
  int result = client.download_file(remotePath, cachePath);
  Debug::msg("Download status: "+result);

  return result == 1;
}

bool CacheService::writeFile(std::string pathName, std::string text,
                             int offset)
{
    Debug::msg("Now writing file");
    Debug::msg("The text which will be written over is: " + text);
  if (checkValidityFetch(pathName))
  {
      Debug::msg("File in cache is now valid");
    // now perform write to the cache file
    if (fh.WriteFile(getLocalPathToFile(pathName).c_str(), text.c_str(),
                     offset))
    {
      // inform the server about the change
        Debug::msg("Local write success");
      client.write_file(pathName, text.c_str(), offset);
      time_t servertime = client.fetch_last_modified_time(pathName);
      updateCacheMap(pathName, servertime);
      return true;
    }
    Debug::msg("Local write failure");
  }
  return false;
}

bool CacheService::appendFile(std::string pathName, std::string text)
{
    Debug::msg("Now writing file");
    Debug::msg("The text which will be written over is: " + text);
    if (checkValidityFetch(pathName))
    {
        Debug::msg("File in cache is now valid");
        // now perform write to the cache file
        if (fh.WriteFile(getLocalPathToFile(pathName).c_str(), text.c_str(),-1))
        {
            // inform the server about the change
            Debug::msg("Local write success");
 
            client.write_file(pathName, text.c_str(), -1);
            time_t servertime = client.fetch_last_modified_time(pathName);
            updateCacheMap(pathName, servertime);
            return true;
        }
        Debug::msg("Local write failure");
    }
    return false;
}

std::string CacheService::readFile(std::string pathName, int offset,
                                   int bytes)
{
    Debug::msg("Now reading file");
  char echoBuffer[bufferSize];
  if (checkValidityFetch(pathName))
  {
      Debug::msg("File in cache is now valid");
    fh.ReadFile(getLocalPathToFile(pathName).c_str(), echoBuffer, bytes,
                offset);
    string s(echoBuffer);
    return s;
  }
  else
  {
    return "";
  }
}

bool CacheService::checkValidityFetch(std::string pathName)
{
    Debug::msg("Now checking validity of file");
  map<string, File>::iterator it;
  it = cacheMap.find(pathName);

  if (it == cacheMap.end())
  {
    // does not exist in map (thus need to request file from server again before
    // proceeding)
      Debug::msg("File does not exist");
    return fetchFile(pathName);
  }
  else
  {
    // exist in map (need to check freshness)
      Debug::msg("File exists");
    File file = it->second;
    // check if exceed the freshness interval
    // if does not exceed, then is valid
    if (file.computeTimeElapsed() <= freshnessInterval*1.0/(1000*1.0))
    {
        Debug::msg("File time elapsed is within freshness interval");
      return true;
    }
    else
    {
        Debug::msg("File time elapsed is not within freshness interval");
      // check if server side last modified is the same
        Debug::msg("Now checking server and client last modified time");
      time_t last_modified_time = client.fetch_last_modified_time(pathName);

      if (difftime(last_modified_time, file.createdTime) != 0)
      {
          Debug::msg("Server and client last modified time does not match, server has more updated copy");
        return fetchFile(pathName);
      }
      else
      {
          Debug::msg("Server and client last modified time match");
        return true;
      }
    }
  }
}

void CacheService::updateCacheMap(std::string pathName, time_t time)
{
    Debug::msg("Now updating cache map");
  if (cacheMap.find(pathName) == cacheMap.end())
  {
    // does not exist in map
      Debug::msg("File is not in map");
    File file(getLocalPathToFile(pathName), time);
    cacheMap.insert({pathName, file});
  }
  else
  {
    // exist in map
      Debug::msg("File is in map");
    File file(getLocalPathToFile(pathName), time);
    cacheMap.at(pathName) = file;
  }
}

bool CacheService::fetchFile(std::string pathName)
{
  // convert to current directory cache file name
    Debug::msg("Now fetching the file from the server");
  string cachepath = getLocalPathToFile(pathName);

  Debug::msg("Now fetching the last modified time from the server");
  time_t last_modified_time = client.fetch_last_modified_time(pathName);

  
  char last_modified_time_string[80];
  strftime(last_modified_time_string, 20, "%Y-%m-%d %H:%M:%S",
           gmtime(&last_modified_time));
  string s = last_modified_time_string;
  Debug::msg("Last modified time fetched : " + s);
  // write from the server to the cache
  Debug::msg("Now downloading the file");
  if (downloadFile(pathName, cachepath))
  {
      Debug::msg("Download success");
    // update the hashing table
    updateCacheMap(pathName, last_modified_time);
    return true;
  }
  else
  {
      Debug::msg("Download failure");
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
  Debug::msg("Local File Path: "+localPath);
  return localPath;
}

std::string CacheService::getAbsoluteFilePathToMainFolder()
{
  return fs::current_path();
}
