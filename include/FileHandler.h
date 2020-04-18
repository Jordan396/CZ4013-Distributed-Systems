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
#include "Debug.h"
/* response codes for the various situations which might occur in the process of doing i/o operations */
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

  /** 
   * @brief readfile reads from a given file into the buffer and returns a response code.
   * @param fileName absolute filename of the given file
   * @param echoBuffer buffer where the data is read into
   * @param nBytes number of bytes to read
   * @param startPos starting position to read at
   * @return int a response code indicating success or failure
   **/
  int ReadFile(const char *fileName, char echoBuffer[], int nBytes,
               int startPos);
  
  /** 
  * @brief writes a string to a specified filepath at the given offset
  * @param filepath the filepath to write to 
  * @param toWrite the string which has to be written
  * @param offset the offset at which the string will be written
  * @return int response code indicating success or otherwise, reason for failure 
  * */
  int WriteFile(const char *filepath, const char *toWrite, int offset);

  /**
   * @brief clears the file specified by filepath - ie, it will be an empty file
   * @param filepath the filepath of the file to be cleared
   * @return int response code indicating success or otherwise, the cause of failure
   * */
  int ClearFile(const char *filepath);

  /**
   * @brief deletes the file specified by filepath
   * @param filepath the filepath of the file to be deleted
   * @return int response code indicating success or otherwise, the cause of failure
   * */
  int DeleteFile(const char *filename);

  /**
   * @brief this renames a given file to the new name 
   * @param oldname the filepath of the file
   * @param newname the new filepath of the file
   * @return int response code indicating success or otherwise, the cause of failure
   * */
  int Rename(char *oldname, char *newname);

  /**
   * @brief creates a new file with the specified filename
   * @param filename the filepath of the file to be cleared
   * @return int response code indicating success
   * */
  int CreateFile(const char *filename);
  ~FileHandler();
};

#endif