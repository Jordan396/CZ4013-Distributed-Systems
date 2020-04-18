/**
 * @file File.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Implementation code for File.cpp
 */

#include "File.h"

File::File(std::string lfp, time_t ct) : localFilePath(lfp), createdTime(ct) {}

double File::computeTimeElapsed() {
  time_t now;
  time(&now);
  return difftime(now, createdTime);
}

File::~File() {}
  