#pragma once

#include <fstream>
#include <iostream>
#include <experimental/filesystem>

#define ERR_FILE_NOT_EXIST -1;
#define ERR_MEMORY_INSUFFICIENT -2; 
#define ERR_READ -3;
#define ERR_OFFSET_OVERFLOW -4; 
#define ERR_NEGATIVE_OFFSET -5; 
#define ERR_WRITE -6;
#define FILE_WRITE_SUCCESS 1; 

class FileHandler
{
public:
    int ReadFile(char* fileName, char echoBuffer[], int nBytes, int startPos);
    int WriteFile(const char* filepath, const char* toWrite, int offset);
    int ClearFile(char* filepath, char *responseContent);
    int DeleteFile(char * filename);
    int Rename(char * oldname, char * newname);
	~FileHandler();
};