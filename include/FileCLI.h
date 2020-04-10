#pragma once
#include "CacheManager/CacheService.h"
#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
class CacheService;

class FileCLI {
public:
  void readFile();
  void clearFile();
  void clearContent();
  void writeFile();
  void fetchFile();
  void clearCache();
  ~FileCLI();
  FileCLI();

private:
  void listFile();
  bool checkValidity(int fileID);
  int cacheSize;
  CacheService cv;
  std::map<int, std::string> cacheReference;
  std::vector<std::string> cacheKeys;
};
