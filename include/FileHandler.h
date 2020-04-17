/**
 * @file FileHandler.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in FileHandler.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "Global.h"

#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string.h>

#define ERR_FILE_NOT_EXIST -1;
#define ERR_MEMORY_INSUFFICIENT -2;
#define ERR_READ -3;
#define ERR_OFFSET_OVERFLOW -4;
#define ERR_NEGATIVE_OFFSET -5;
#define ERR_WRITE -6;
#define FILE_WRITE_SUCCESS 1;
#define FILE_CREATE_SUCCESS 1;

using namespace std;

class FileHandler
{
public:
  int ReadFile(const char *fileName, char echoBuffer[], int nBytes,
               int startPos);
  int WriteFile(const char *filepath, const char *toWrite, int offset);
  int ClearFile(const char *filepath);
  int DeleteFile(const char *filename);
  int Rename(char *oldname, char *newname);
  int CreateFile(const char *filename);
  ~FileHandler();
};

#endif