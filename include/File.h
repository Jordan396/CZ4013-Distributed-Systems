/**
 * @file File.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in File.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef FILE_O_H
#define FILE_O_H

#include "time.h"
#include <chrono>
#include <string>

class File
{
public:
  File(std::string lfp, time_t ct);
  std::string localFilePath;
  time_t createdTime;
  double computeTimeElapsed();
  ~File();
};

#endif