  
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
void parse_message(cJSON *jobjReceived);

// TODO: Integrate the hardcoded variables below with command prompt parser 
// Variables relating to server addressing
char *servAddressHardcode = "172.21.148.168";
unsigned short servPortHardcode = Socket::resolveService("2222", "udp");


const int BUFFER_SIZE = 255;     // Longest string to echo

/* Variables to handle transfer of data */
  cJSON *jobjToSend;              /* JSON payload to be sent */
  cJSON *jobjReceived;            /* JSON response received */
  char objReceived[BUFFER_SIZE]; /* String response received */

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

      parse_message(jobjReceived);
    }
  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  // NOT REACHED

  return 0;
}

/** \copydoc parse_message */
void parse_message(cJSON *jobjReceived)
{
  int clientCommandCode = cJSON_GetObjectItemCaseSensitive(jobjReceived, "clientCommandCode")->valueint;
  if ((clientCommandCode == 0) || (clientCommandCode == 1)){
    int offset = cJSON_GetObjectItemCaseSensitive(jobjReceived, "offset")->valueint;
    int nBytes = cJSON_GetObjectItemCaseSensitive(jobjReceived, "nBytes")->valueint;
    char *filepath = cJSON_GetObjectItemCaseSensitive(jobjReceived, "filepath")->valuestring;
  }
}


// readfile is for use by the server, reads from a given file to a standard writer and returns number of bytes read 
// assumption made is that we either specify FULL file path or it exists in current directory where server is executing 
// writer is from golang, interface that has write method (the connection)

// int ReadFile(string fileName, Writer writer, int startPos = 0) {
//   // file opening logic can be abstracted away for reuse
//   // first we check if file exists 
//   FILE * pFile; 
//   pFile = fopen(fileName, "r");
//   if pFile == NULL { // file requested does not exist, we return error back to client 
//       return -1; // server calling this function has to check err code 
//   }
//   // advance start pointer to requested position to begin reading; if client does not specify assume start of file 
//   fseek(pFile, startPos, SEEK_SET);
//   long lSize;
//   char * buffer;
//   size_t result;

//   lSize = ftell (pFile);
//   // allocate memory to contain the whole file
//   buffer = (char*) malloc (sizeof(char)*lSize); 
//   if (buffer == NULL) { // no memory to allocate buffer: return error code to client 
//     fputs ("Memory error", writer); 
//     return -2; 
//   }
  
//   result = fread (buffer, 1, lSize, pFile); // pFile advance dto startPos 
//   if (result != lSize) {
//     fputs ("Reading error",stderr); 
//     return -3; 
//     }
//   fclose (pFile);
//   free (buffer);
//   return result; 
// }
