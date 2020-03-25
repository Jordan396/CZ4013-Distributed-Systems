#pragma once

#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <string.h>

#include "Global.h"

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
    int ReadFile(const char* fileName, char echoBuffer[], int nBytes, int startPos);
    int WriteFile(const char* filepath, const char* toWrite, int offset);
    int ClearFile(char* filepath);
    int DeleteFile(char * filename);
    int Rename(char * oldname, char * newname);
    int CreateFile(const char * filename);
	~FileHandler();
};