/**
 * @file CacheService.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in CacheService.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef CACHE_SERVICE_H
#define CACHE_SERVICE_H

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
#include "Debug.h"

class CacheService
{
public:
  /**
   * @brief write the file
   * Method will first update the file in the local file directory and then update the server of the new file copy
   * @param pathName remote pathName
   * @param text text to be written into the file
   * @param offset the offset of the file at which the append should start
   * @return true if success, false if failure
   */
  bool writeFile(std::string pathName, std::string text, int offset);

  /**
   * @brief read the file based on the offset and the number of bytes to be read
   * @param pathName remote pathName
   * @param offset the offset of the file where reading should start
   * @param bytes the number of bytes from the offset to be read
   * @return string of text which is read, or error message if the reading fails
   */
  std::string readFile(std::string pathName, int offset, int bytes);

  /**
   * @brief clear the copy of file in local directory
   * Method will remove the the file in the local directory and update the hashmap accordingly
   * @param pathName remote pathName
   * @return true if clear copy of file is successful, false if it fails
   */
  bool clearFile(std::string pathName);

  /**
   * @brief register the client with the server and listen for new update to the file
   * @param pathName remote pathName
   * @param monitorDuration the time period for monitoring
   * @return true if success, false if failure
   */
  bool monitorFile(std::string pathName, int monitorDuration);

  /**
   * @brief clear the content in the specified file
   * Method will update both the local and remote copies of the file
   * @param pathName remote pathName
   * @return true if success, false if failure
   */
  bool clearContent(std::string pathName);

  /**
   * @brief download the remote file into the local file directory
   * Method will call RFAcli's download_file method 
   * @param remotePath remote pathName
   * @param cachePath local pathName
   * @return true if success, false if failure
   */
  bool downloadFile(std::string remotePath, std::string cachePath);

  /**
   * @brief clear all the files stored in the local cache directory
   * @param void
   * @return true if success, false if failure
   */
  bool clearCache();

  /**
   * @brief list all the files in the local cache directory and recorded in the cache hashmap
   * @param void
   * @return vector<string> vector of the different types of file names
   */
  std::vector<std::string> listCache();

  /**
   * @brief check whether the file in the local directory is valid, if not fetch the file again from the server to ensure validity
   * @param pathName remote pathName
   * @return true if the validity check and its operations are successful, false if any of them fails
   */
  bool checkValidityFetch(std::string pathName);

  CacheService();
  ~CacheService();

private:
  FileHandler fh;
  RFAcli client;
  std::map<std::string, File> cacheMap;

  /**
   * @brief fetch the file from the remote server to the local file directory and update the last_modified_time record in hashmap
   * @param pathName remote pathName
   * @return true if fetch succeed, false if fetch fails
   */
  bool fetchFile(std::string pathName);

  /**
   * @brief get the local file path 
   * @param fileName remote pathName
   * @return string of the local file path
   */
  std::string getLocalPathToFile(std::string fileName);

  /**
   * @brief extract the file name from the remote file path
   * @param remoteFilePath remote pathName
   * @return string of the file name
   */
  std::string extractFileName(std::string remoteFilePath);

  /**
   * @brief get the absolute file path to main folder 
   * @param void
   * @return string of the absolute file path
   */
  std::string getAbsoluteFilePathToMainFolder();

  /**
   * @brief update the entry in the cache hashmap
   * @param pathName remote pathName
   * @param time last_modified_time of the file
   * @return void
   */
  void updateCacheMap(std::string pathName, time_t time);
};

#endif