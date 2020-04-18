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
  /* variables describing the file object */
  std::string localFilePath;
  time_t createdTime;

  /**
   * @brief Construct a new File:: File object
   * 
   * @param lfp Local file path
   * @param ct  Time created
   */
  File(std::string lfp, time_t ct);

  /** 
   * @brief computes the time elapsed betweeen the current time and the time since the file struct was first created 
   * @param void
   * @return void 
  * */
  double computeTimeElapsed();

  /**
   * @brief Destroy the File object
   * 
   */
  ~File();
};

#endif