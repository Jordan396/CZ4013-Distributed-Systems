  
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
  */

#include "./../dependencies/PracticalSocket.h" // For UDPSocket and SocketException
#include <cstring>
#include <iostream>          // For cout and cerr
#include <cstdlib>           // For atoi()
#include <stdlib.h>

const int ECHOMAX = 255;     // Longest string to echo

int main(int argc, char *argv[]) {

  if (argc != 2) {                  // Test for correct number of parameters
    cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
    exit(1);
  }

  unsigned short echoServPort = atoi(argv[1]);     // First arg:  local port

  try {
    UDPSocket sock(echoServPort);                
  
    char echoBuffer[ECHOMAX];         // Buffer for echo string
    int recvMsgSize;                  // Size of received message
    string sourceAddress;             // Address of datagram source
    unsigned short sourcePort;        // Port of datagram source
    for (;;) {  // Run forever
      // Block until receive message from a client
      recvMsgSize = sock.recvFrom(echoBuffer, ECHOMAX, sourceAddress, 
                                      sourcePort);
  
      cout << "Received packet from " << sourceAddress << ":" 
           << sourcePort << endl;
  
      sock.sendTo(echoBuffer, recvMsgSize, sourceAddress, sourcePort);
    }
  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  // NOT REACHED

  return 0;
}

// readfile is for use by the server, reads from a given file to a standard writer and returns number of bytes read 
// assumption made is that we either specify FULL file path or it exists in current directory where server is executing 
// writer is from golang, interface that has write method (the connection)
int ReadFile(string fileName, Writer writer, int startPos = 0) {
  // file opening logic can be abstracted away for reuse
  // first we check if file exists 
  FILE * pFile; 
  pFile = fopen(fileName, "r");
  if pFile == NULL { // file requested does not exist, we return error back to client 
      return -1; // server calling this function has to check err code 
  }
  // advance start pointer to requested position to begin reading; if client does not specify assume start of file 
  fseek(pFile, startPos, SEEK_SET);
  long lSize;
  char * buffer;
  size_t result;

  lSize = ftell (pFile);
  // allocate memory to contain the whole file
  buffer = (char*) malloc (sizeof(char)*lSize); 
  if (buffer == NULL) { // no memory to allocate buffer: return error code to client 
    fputs ("Memory error", writer); 
    return -2; 
  }
  
  result = fread (buffer, 1, lSize, pFile); // pFile advance dto startPos 
  if (result != lSize) {
    fputs ("Reading error",stderr); 
    return -3; 
    }
  fclose (pFile);
  free (buffer);
  return result; 
}
