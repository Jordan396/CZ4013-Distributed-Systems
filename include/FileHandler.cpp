#include "FileHandler.h"

// readfile is for use by the server, reads from a given file to a standard writer and returns number of bytes read 
// assumption made is that we either specify FULL file path or it exists in current directory where server is executing 
int FileHandler::ReadFile(char* fileName, char echoBuffer[], int nBytes, int startPos = 0) { // we write to a buffer 
  // file opening logic can be abstracted away for reuse
  // first we check if file exists 
  FILE * pFile; 

  /*
  int length = fileName.length(); 
  char char_array[length + 1];
  strcpy(char_array, fileName.c_str()); 
  pFile = fopen(char_array, "rb");
  */
  
  pFile = fopen(fileName, "rb");
  if (pFile == NULL) { // file requested does not exist, we return error back to client 
      sprintf (echoBuffer, "%s", "File does not exist"); 
      return ERR_FILE_NOT_EXIST; // server calling this function has to check err code 
  }

  long lsize;
  fseek (pFile, 0, SEEK_END);   
  lsize = ftell (pFile); // get size of the file 

  if (lsize >  BUFFER_SIZE) { // no memory to allocate buffer: return error code to client 
      sprintf (echoBuffer, "%s", "Memory error"); 
      return ERR_MEMORY_INSUFFICIENT; 
    }

  // check whether >= 0
  if (startPos < 0) { 
    sprintf(echoBuffer, "%s", "The required starting position is less than 0");
    return ERR_NEGATIVE_OFFSET;
  } 

  // check whether startPos > maxlength 
  if (startPos > lsize) { 
    sprintf(echoBuffer, "%s", "The specified offset is greater than the length of the file");
    return ERR_OFFSET_OVERFLOW;
  }
  
  fseek(pFile, startPos, SEEK_SET);
  size_t result;
  
  // no checking of nBytes
  result = fread (echoBuffer, 1, nBytes, pFile); // pFile advanced to startPos 
  if (result != lsize) {
    sprintf (echoBuffer, "%s", "Reading error");  
    return ERR_READ; 
    }
  fclose (pFile);
  return result; 
}

// writefile will write to a file pointed at by filepath at offset 0 
int FileHandler::WriteFile(const char* filepath, const char* toWrite, int offset = 0) {
  // first we check if file exists 
  FILE * pFile; 

  pFile = fopen(filepath, "r+");
  if (pFile == NULL) { // file requested does not exist, we return error back to client 
      return ERR_FILE_NOT_EXIST; // server calling this function has to check err code 
  }

  long lsize;
  fseek (pFile, 0, SEEK_END);   
  lsize = ftell (pFile); // get size of the file 
  cout << "Size of the file is " << lsize << endl; 

  // check whether startPos > maxlength 
  if (offset > lsize) { 
    return ERR_OFFSET_OVERFLOW; 
  }

  // check whether >= 0
  if (offset < 0) { 
    offset = offset + lsize + 1; 
  } 

  size_t length = strlen(toWrite); // get number of elements from array pointer 
  cout << "length is " << length << endl; 
  // overwrite with toWrite + [offset:] of original file 
  char originalFile [lsize + length]; // allocate length of string to be written + filesize to ensure capacity
  fseek(pFile, offset, SEEK_SET);
  strcpy(originalFile, toWrite);
  char temp[lsize - offset]; // holds off set till end 
  fread(temp, 1, lsize - offset, pFile); 
  strcat(originalFile, temp);
  fseek(pFile, offset, SEEK_SET);
  int written = fwrite(originalFile, 1, (lsize-offset)+length, pFile);
  if (written != (lsize-offset)+length) {
    return ERR_WRITE;
  } 

  fclose (pFile);
  return FILE_WRITE_SUCCESS; 
}

// check if file exists - if yes, overwrite else return error 
int FileHandler::ClearFile(char* filepath, char *responseContent) {
  FILE * pFile; 

  pFile = fopen(filepath, "r");
  if (pFile == NULL) { // file requested does not exist, we return error back to client 
      strcpy(responseContent, "File does not exist"); 
      return ERR_FILE_NOT_EXIST; // server calling this function has to check err code 
  }
  fclose(pFile);

  // file exists, write 
  pFile = fopen(filepath, "w");
  fclose(pFile);
  return 1; 
}

// wrapper around remove provided by cpp 
int FileHandler::DeleteFile(char * filename) { 
  int a = remove(filename);
  // file deletion unsucessful
  if (a != 0) { 
    return ERR_FILE_NOT_EXIST;
  }
  return a; // 0 
}

// wrapper around rename provided by cpp
int FileHandler::Rename(char * oldname, char * newname) { 
  int a = rename(oldname, newname); 
  if (a != 0) { 
    return ERR_FILE_NOT_EXIST;
  }
  return a; // 0 
}


// taken from stackoverflow - decorator pattern in cpp???
// call it as decorate(int, func)(params)
// template<class T>
// auto decorator(int lossRate, T&& func) {
//     int percent = rand() % 100;
//     auto new_function = [func = std::forward<T>(func)](auto&&... args) {
//         auto result = func(std::forward<decltype(args)>(args)...);   
//         if (percent > lossRate) { 
//           return result; 
//         }
//         return;
//     };
//     return new_function;
// } 

// }

FileHandler::~FileHandler()
{
}