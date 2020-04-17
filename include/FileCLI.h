/**
 * @file FileCLI.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in FileCLI.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef FILE_CLI_H
#define FILE_CLI_H

#include "CacheService.h"

#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

class FileCLI
{
public:
  void readFile();
  void clearFile();
  void clearContent();
  void writeFile();
  void fetchFile();
  void clearCache();
  void monitorFile();
  ~FileCLI();
  FileCLI();

private:
  void listFile();
  bool checkValidity(string fileID);
  int cacheSize;
  CacheService cv;
  std::map<int, std::string> cacheReference;
  std::vector<std::string> cacheKeys;
  bool isNumber(std::string s);
};

#endif