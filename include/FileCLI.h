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
  /**
   * @brief Executed when the user wants to read the file, method will read the users' input for file name, offset and number of bytes to be read
   * Method will call CacheService readFile method 
   * @param void
   * @return void
   */
  void readFile();

  /**
   * @brief Executed when the user wants to clear the file in the cache, method will read the users' input for file name
   * Method will call CacheService clearFile method 
   * @param void
   * @return void
   */
  void clearFile();

  /**
   * @brief Executed when the user wants to clear the content of the file, method will read the users' input for file name
   * Method will call CacheService clearContent method 
   * @param void
   * @return void
   */
  void clearContent();

  /**
   * @brief Executed when the user wants to write the file, method will read the users' input for file name, the offset and the string to be appended
   * Method will call CacheService writeFile method 
   * @param void
   * @return void
   */
  void writeFile();

  /**
   * @brief Executed when the user wants to fetch the file from the remote source, method will read the users' input for file path
   * Method will call CacheService checkValidityCheck method 
   * @param void
   * @return void
   */
  void fetchFile();

  /**
   * @brief Executed when the user wants to clear the cache
   * Method will call CacheService clearCache method 
   * @param void
   * @return void
   */
  void clearCache();

  /**
   * @brief Executed when the user wants to monitor the file, method will read the users' input for file name and monitoring duration
   * Method will call CacheService monitorFile method 
   * @param void
   * @return void
   */
  void monitorFile();
  ~FileCLI();
  FileCLI();

private:

  /* cache and the variables used by cache which the command line interface requires */
  int cacheSize;
  CacheService cv;
  std::map<int, std::string> cacheReference;
  std::vector<std::string> cacheKeys;

  /**
   * @brief Called by methods which require selection of files in cache, method will display all the files in the local cache, and map them to their fileIDs
   * @param void
   * @return void
   */
  void listFile();

  /**
   * @brief Called when the user inputs a fileID to check whether the fileID is valid
   * @param fileID
   * @return true if is valid, false if is invalid
   */
  bool checkValidity(string fileID);

  /**
   * @brief Check whether the string is numerical
   * @param s string
   * @return true if is numerical, false if it contains other non-numerical characters
   */
  bool isNumber(std::string s);
};

#endif