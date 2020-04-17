#pragma once
#include "FileHandler.h"
#include "Global.h"
#include "RFAcli.h"
#include "File.h"

#include <chrono>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/serialization/map.hpp>

class CacheService {
public:
  // this method will be called when client wants to write a file
  bool writeFile(std::string pathName, std::string text, int offset);

  // this method will be called when client wants to read a file
  std::string readFile(std::string pathName, int offset, int bytes);

  // this method will be called when client wants to delete a particular cache
  // entry
  bool clearFile(std::string pathName);

  // this method will be called when the user wants to monitor the file for updates
  bool monitorFile(std::string pathName, int monitorDuration);

  // thid method will be called when client wants to clear the content of the
  // file
  bool clearContent(std::string pathName);

  bool downloadFile(std::string remotePath, std::string cachePath);

  // this method will be called when client wants to clear the whole cache
  bool clearCache();

  // this method will list all the entries in the cache
  std::vector<std::string> listCache();

  // this method will fetch the file to the cache (checking if it is in the
  // cache and whether it is valid first)
  bool checkValidityFetch(std::string pathName);

  CacheService();
  ~CacheService();

private:
  FileHandler fh;
  RFAcli client;
  std::map<std::string, File> cacheMap;

  // this method will call writeAll and also update the cacheMap to include the
  // time of creation of file
  bool fetchFile(std::string pathName);

  // for resolving file path
  std::string getLocalPathToFile(std::string fileName);
  std::string extractFileName(std::string remoteFilePath);
  std::string getAbsoluteFilePathToMainFolder();

  // Chong Yan comment
  void updateCacheMap(std::string pathName, time_t time);
};
