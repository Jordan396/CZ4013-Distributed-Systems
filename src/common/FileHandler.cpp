/**
 * @file FileHandler.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Implementation code for FileHandler.cpp
 */

#include "FileHandler.h"

int FileHandler::ReadFile(const char *fileName, char echoBuffer[], int nBytes,
                          int startPos = 0)
{ // we write to a buffer
  // file opening logic can be abstracted away for reuse
  // first we check if file exists
  FILE *pFile;

  /*
  int length = fileName.length();
  char char_array[length + 1];
  strcpy(char_array, fileName.c_str());
  pFile = fopen(char_array, "rb");
  */
  Debug::msg("startpos: " + to_string(startPos));
  Debug::msg("nbytes: " + to_string(nBytes));
  pFile = fopen(fileName, "rb");
  if (pFile ==
      NULL)
  { // file requested does not exist, we return error back to client
    sprintf(echoBuffer, "%s", "File does not exist");
    return ERR_FILE_NOT_EXIST; // server calling this function has to check err
                               // code
  }

  long lsize;
  fseek(pFile, 0, SEEK_END);
  lsize = ftell(pFile); // get size of the file

  // @JiaChin: Assume buffer provided is always smaller than lsize
  // Idea is to transfer by file by chunks of buffer size
  // if (lsize > bufferSize) { // no memory to allocate buffer: return error
  // code to client
  //   sprintf (echoBuffer, "%s", "Memory error");
  //   return ERR_MEMORY_INSUFFICIENT;
  // }

  // check whether >= 0
  if (startPos < 0)
  {
    sprintf(echoBuffer, "%s", "The required starting position is less than 0");
    return ERR_NEGATIVE_OFFSET;
  }

  // check whether startPos > maxlength
  if (startPos > lsize)
  {
    sprintf(echoBuffer, "%s",
            "The specified offset is greater than the length of the file");
    return ERR_OFFSET_OVERFLOW;
  }
  // check whether startPos > maxlength
  if (startPos == lsize)
  {
    echoBuffer[0] = '\0';
    return 0;
  }

  fseek(pFile, startPos, SEEK_SET);
  size_t result;

  // no checking of nBytes
  result = fread(echoBuffer, 1, nBytes, pFile); // pFile advanced to startPos
  if (result != nBytes)
  {
    if (ferror(pFile))
    {
      sprintf(echoBuffer, "%s", "Reading error");
      return ERR_READ;
    }
  }
  echoBuffer[result] = '\0';
  fclose(pFile);
  return result;
}

int FileHandler::WriteFile(const char *filepath, const char *toWrite,
                           int offset = 0)
{
  // first we check if file exists
  FILE *pFile;

  pFile = fopen(filepath, "r+");
  if (pFile ==
      NULL)
  {                            // file requested does not exist, we return error back to client
    return ERR_FILE_NOT_EXIST; // server calling this function has to check err
                               // code
  }

  long lsize;
  fseek(pFile, 0, SEEK_END);
  lsize = ftell(pFile); // get size of the file
  // cout << "Size of the file is " << lsize << endl;

  // check whether startPos > maxlength
  if (offset > lsize)
  {
    return ERR_OFFSET_OVERFLOW;
  }

  // check whether >= 0
  if (offset < 0)
  {
    offset = offset + lsize + 1;
  }

  size_t length = strlen(toWrite); // get number of elements from array pointer
  // cout << "length is " << length << endl;

  // overwrite with toWrite + [offset:] of original file
  int originalFileSize = lsize - offset + length;
  int tempSize = lsize - offset;
  char originalFile[originalFileSize]; // allocate length of string to be
                                       // written + filesize to ensure capacity
  fseek(pFile, offset, SEEK_SET);
  char temp[tempSize]; // holds offset till end
  fread(temp, 1, lsize - offset, pFile);

  // Transfer toWrite and remaining segment to originalFile
  int charIdx;
  for (charIdx = 0; charIdx < length; charIdx++)
  {
    originalFile[charIdx] = toWrite[charIdx];
  }
  for (int i = 0; i < tempSize; i++)
  {
    originalFile[charIdx] = temp[i];
    charIdx++;
  }
  fseek(pFile, offset, SEEK_SET);

  int written = fwrite(originalFile, 1, originalFileSize, pFile);
  if (written != originalFileSize)
  {
    return ERR_WRITE;
  }

  fclose(pFile);
  return FILE_WRITE_SUCCESS;
}

int FileHandler::ClearFile(const char *filepath)
{
  FILE *pFile;

  pFile = fopen(filepath, "r");
  if (pFile ==
      NULL)
  {                            // file requested does not exist, we return error back to client
    return ERR_FILE_NOT_EXIST; // server calling this function has to check err
                               // code
  }
  fclose(pFile);

  // file exists, write
  pFile = fopen(filepath, "w");
  fclose(pFile);
  return 1;
}

int FileHandler::DeleteFile(const char *filename)
{
  int a = remove(filename);
  // file deletion unsucessful
  if (a != 0)
  {
    return ERR_FILE_NOT_EXIST;
  }
  return a; // 0
}

int FileHandler::Rename(char *oldname, char *newname)
{
  int a = rename(oldname, newname);
  if (a != 0)
  {
    return ERR_FILE_NOT_EXIST;
  }
  return a; // 0
}

int FileHandler::CreateFile(const char *filename)
{
  std::ofstream output(filename);
  output.close();
  return FILE_CREATE_SUCCESS;
}

FileHandler::~FileHandler() {}
