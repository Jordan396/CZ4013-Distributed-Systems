/**
 * @file File.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Implementation code for File.cpp
 */

#include "File.h"

File::File(std::string lfp, time_t ct) : localFilePath(lfp), createdTime(ct) {}

/** 
 * @brief computes the time elapsed betweeen the current time and the time since the file struct was first created 
 * @param void
 * @return void 
 * */
double File::computeTimeElapsed() {
  time_t now;
  time(&now);
  return difftime(now, createdTime);
}

File::~File() {}
  