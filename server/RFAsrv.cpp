  
/****************************************************************************
 * @author:                                                                 *
 * - Jordan396 <https://github.com/Jordan396/>                              *
 * - leechongyan <https://github.com/leechongyan/>                          *
 * - seaerchin <https://github.com/seaerchin/>                              *
 *                                                                          *
 * This file was heavily adapted from the source code in:                   *
 *   "TCP/IP Sockets in C: Practical Guide for Programmers"                 *
 *   by Michael J. Donahoo and Kenneth L. Calvert                           *
 *   <http://cs.baylor.edu/~donahoo/practical/CSockets/textcode.html>       *
 *                                                                          *
 ****************************************************************************/

 /**
  * @file server.cpp
  * @author Jordan396, leechongyan, seaerchin
  * @date xxx 2020
  * @brief .
  *
  * 
  * TODO:
  * - Integrate readfile with cJSON
  * 
  */

#include "./RFAsrv.h"

/* Function declarations */
int get_client_command_code(cJSON *jobjReceived);
void readFile(char* filepath, int offset, int nBytes, char* responseContent);
void writeFile(char* filepath, int offset, int nBytes, char *responseContent);
int get_client_command_code(cJSON *jobjReceived);
int get_offset(cJSON *jobjReceived);
int get_nBytes(cJSON *jobjReceived);
void get_filepath(cJSON *jobjReceived, char *filepath);
char* get_toWrite(cJSON *jobjReceived);
int ReadFile(char* fileName, char echoBuffer[], int nBytes, int startPos);
int WriteFile(char* filepath, char* toWrite, char *responseContent, int offset); 
// TODO: Integrate the hardcoded variables below with command prompt parser 
// Variables relating to server addressing
char *servAddressHardcode = "172.21.148.168";
unsigned short servPortHardcode = Socket::resolveService("2222", "udp");


const int BUFFER_SIZE = 255;     // Longest string to echo

/* Variables to handle transfer of data */
cJSON *jobjToSend;              /* JSON payload to be sent */
cJSON *jobjReceived;            /* JSON response received */
char objReceived[BUFFER_SIZE]; /* String response received */
char responseContent[BUFFER_SIZE];

/* Variables for commands */
char filepath[200];

int main(int argc, char *argv[]) {

  // if (argc != 2) {                  // Test for correct number of parameters
  //   cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
  //   exit(1);
  // }

  // unsigned short echoServPort = atoi(argv[1]);     // First arg:  local port

  try {
    UDPSocket sock(servPortHardcode);                
  
    char serverBuffer[BUFFER_SIZE];         // Buffer for echo string
    int recvMsgSize;                  // Size of received message
    string sourceAddress;             // Address of datagram source
    unsigned short sourcePort;        // Port of datagram source
    for (;;) {  // Run forever
      // Block until receive message from a client
      recvMsgSize = sock.recvFrom(serverBuffer, BUFFER_SIZE, sourceAddress, sourcePort);

      cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;
  
      // sock.sendTo(echoBuffer, recvMsgSize, sourceAddress, sourcePort);
      
      strncpy(objReceived, serverBuffer, sizeof(serverBuffer));
      jobjReceived = cJSON_Parse(objReceived);

      // Debugging
      cout << cJSON_Print(jobjReceived) << endl;

      switch (get_client_command_code(jobjReceived)){
        case READ_CMD_CODE:
          cout << "Executing read command..." << endl;
          get_filepath(jobjReceived, filepath);
          // readFile(filepath, get_offset(jobjReceived), get_nBytes(jobjReceived), responseContent);
          // get nBytes, which can be a defensive check, is thrown away 
          ReadFile(filepath, responseContent, get_nBytes(jobjReceived), get_offset(jobjReceived)); 
          cout << "responseContent: " << responseContent << endl;
          break;
        case WRITE_CMD_CODE:
          cout << "Executing write command..." << endl;
          get_filepath(jobjReceived, filepath);
          WriteFile(filepath, get_toWrite(jobjReceived), responseContent, get_offset(jobjReceived));
          cout << "responseContent: " << responseContent << endl;
          break;
        case MONITOR_CMD_CODE:
          cout << "Executing modify command..." << endl;
          break;
      }
    }
  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  return 0;
}

/** \copydoc get_client_command_code */
int get_client_command_code(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "clientCommandCode")->valueint;
}

int get_offset(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "offset")->valueint;
}

int get_nBytes(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "nBytes")->valueint;
}

void get_filepath(cJSON *jobjReceived, char *filepath)
{
  strcpy(filepath ,cJSON_GetObjectItemCaseSensitive(jobjReceived, "rfaPath")->valuestring);
}

char* get_toWrite(cJSON *jobjReceived) {
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "toWrite")->valuestring;
}

// readfile is for use by the server, reads from a given file to a standard writer and returns number of bytes read 
// assumption made is that we either specify FULL file path or it exists in current directory where server is executing 
int ReadFile(char* fileName, char echoBuffer[], int nBytes, int startPos = 0) { // we write to a buffer 
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
      return -1; // server calling this function has to check err code 
  }

  long lsize;
  fseek (pFile, 0, SEEK_END);   
  lsize = ftell (pFile); // get size of the file 

  if (lsize >  BUFFER_SIZE) { // no memory to allocate buffer: return error code to client 
      sprintf (echoBuffer, "%s", "Memory error"); 
      return -2; 
    }

  // check whether >= 0
  if (startPos < 0) { 
    sprintf(echoBuffer, "%s", "The required starting position is less than 0");
    return -5;
  } 

  // check whether startPos > maxlength 
  if (startPos > lsize) { 
    sprintf(echoBuffer, "%s", "The specified offset is greater than the length of the file");
    return -4;
  }
  
  fseek(pFile, startPos, SEEK_SET);
  size_t result;
  
  // no checking of nBytes
  result = fread (echoBuffer, 1, nBytes, pFile); // pFile advanced to startPos 
  if (result != lsize) {
    sprintf (echoBuffer, "%s", "Reading error");  
    return -3; 
    }
  fclose (pFile);
  return result; 
}

void readFile(char* filepath, int offset, int nBytes, char *responseContent){
  cout << "filepath: " << filepath << endl;
  cout << "offset: " << offset << endl;
  cout << "nBytes: " << nBytes << endl;
  strcpy(responseContent,"It's all yours, Jia Chin! Jiayou!");
  cout << "responseContent: " << responseContent << endl;
}

void writeFile(char* filepath, int offset, int nBytes, char *responseContent){
  cout << "filepath: " << filepath << endl;
  cout << "offset: " << offset << endl;
  cout << "nBytes: " << nBytes << endl;
  strcpy(responseContent,"It's all yours, Jia Chin! Jiayou!");
  cout << "responseContent: " << responseContent << endl;
}


// writefile will write to a file pointed at by filepath at offset 0 
int WriteFile(char* filepath, char* toWrite, char *responseContent, int offset = 0) {
  // first we check if file exists 
  FILE * pFile; 

  pFile = fopen(filepath, "r+");
  if (pFile == NULL) { // file requested does not exist, we return error back to client 
      strcpy(responseContent, "File does not exist"); 
      return -1; // server calling this function has to check err code 
  }

  long lsize;
  fseek (pFile, 0, SEEK_END);   
  lsize = ftell (pFile); // get size of the file 
  cout << "Size of the file is " << lsize << endl; 

  // check whether startPos > maxlength 
  if (offset > lsize) { 
    strcpy(responseContent, "Starting offset exceeded file size! Please use negative indexing to start writing from the end!");
    return -2; 
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
  cout << temp << endl; 
  strcat(originalFile, temp);
  fseek(pFile, offset, SEEK_SET);
  int written = fwrite(originalFile, 1, (lsize-offset)+length, pFile);
  if (written != (lsize-offset)+length) {
    strcpy(responseContent, "Writing error");
  } else {
    strcpy(responseContent, originalFile);
  }
  fclose (pFile);
  return 0; 
}