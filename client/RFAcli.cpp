  
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
    //test git
 /**
  * @file client.cpp
  * @author Jordan396, leechongyan, seaerchin
  * @date xxx 2020
  * @brief .
  *
  * 
  */

#include "./RFAcli.h"

using namespace std;

// TODO: Integrate the hardcoded variables below with command prompt parser 
// Variables are used to test read/write file access on server
char *filepathHardcode = "RFA://Documents/Cinderella.txt"; // path to file on server
int offsetHardcode = 0;  // offset to start R/W from
int nBytesHardcode = 20; // number of bytes to R/W
// Variables relating to server addressing
char *servAddressHardcode = "172.21.148.168";
unsigned short servPortHardcode = Socket::resolveService("2222", "udp");


/* Function declarations */
void format_read_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes);
void format_write_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes);

const int BUFFER_SIZE = 255;     // Longest string to echo

 /* Variables to handle transfer of data */
cJSON *jobjToSend;              /* JSON payload to be sent */
cJSON *jobjReceived;            /* JSON response received */
char objReceived[BUFFER_SIZE]; /* String response received */

int main(int argc, char *argv[]) {
  // To be replaced by Chong Yan
  // if ((argc < 3) || (argc > 4)) {   // Test for correct number of arguments
  //   cerr << "Usage: " << argv[0] 
  //        << " <Server> <Echo String> [<Server Port>]\n";
  //   exit(1);
  // }

  // string servAddress = argv[1];             // First arg: server address
  // char* echoString = argv[2];               // Second arg: string to echo
  // int echoStringLen = strlen(echoString);   // Length of string to echo
  // if (echoStringLen > ECHOMAX) {    // Check input length
  //   cerr << "Echo string too long" << endl;
  //   exit(1);
  // }
  // unsigned short servPort = Socket::resolveService(
  //   (argc == 4) ? argv[3] : "RFA", "udp");
  

  try {
    UDPSocket sock;

    // Create message to send
    jobjToSend = cJSON_CreateObject();
    format_read_message(jobjToSend, filepathHardcode, offsetHardcode, nBytesHardcode);
    // format_write_message(jobjToSend, filepathHardcode, offsetHardcode, nBytesHardcode);

    // Debugging
    cout << cJSON_Print(jobjToSend) << endl;

    // Send the message to the server
    char *message = cJSON_Print(jobjToSend);
    sock.sendTo(message, strlen(message), servAddressHardcode, servPortHardcode);
  
    // // Receive a response
    // char echoBuffer[ECHOMAX + 1];       // Buffer for echoed string + \0
    // int respStringLen;                  // Length of received response
    // if ((respStringLen = sock.recv(echoBuffer, ECHOMAX)) != echoStringLen) {
    //   cerr << "Unable to receive" << endl;
    //   exit(1);
    // }
  
    // echoBuffer[respStringLen] = '\0';             // Terminate the string!
    // cout << "Received: " << echoBuffer << endl;   // Print the echoed arg

    // Destructor closes the socket

  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}

// /** \copydoc waitFor */
// void wait_for(unsigned int secs)
// {
//   unsigned int retTime = time(0) + secs; // Get finishing time.
//   while (time(0) < retTime)
//     ; // Loop until it arrives.
// }

// /** \copydoc receive_message */
// void receive_message(int sock, char *objReceived)
// {
//   int bytesToRecv = 0;
//   int responseIdx = 0;
//   char buffer[RCV_BUF_SIZE];   /* Buffer for ttweet string */
//   char response[MAX_RESP_LEN]; /* Stores the entire response */

//   while (bytesToRecv <= 0)
//   {
//     recv(sock, buffer, RCV_BUF_SIZE, 0);
//     bytesToRecv = atoi(buffer);
//     wait_for(3);
//   }

//   while (bytesToRecv > 0)
//   {
//     if (bytesToRecv > RCV_BUF_SIZE)
//     {
//       recv(sock, buffer, RCV_BUF_SIZE, 0);
//       strncpy(response + responseIdx, buffer, RCV_BUF_SIZE);
//       responseIdx += RCV_BUF_SIZE;
//     }
//     else
//     {
//       recv(sock, buffer, bytesToRecv, 0);
//       strncpy(response + responseIdx, buffer, bytesToRecv);
//       responseIdx += bytesToRecv;
//     }
//     bytesToRecv -= RCV_BUF_SIZE;
//   }
//   strncpy(objReceived, response, sizeof(response));
// }

/** \copydoc format_read_message */
void format_read_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes)
{
  cJSON_AddItemToObject(jobjToSend, "clientCommandCode", cJSON_CreateNumber(READ_CMD_CODE)); 
  cJSON_AddItemToObject(jobjToSend, "offset", cJSON_CreateNumber(offset)); 
  cJSON_AddItemToObject(jobjToSend, "nBytes", cJSON_CreateNumber(nBytes)); 
  cJSON_AddItemToObject(jobjToSend, "rfaPath", cJSON_CreateString(filepath)); 
}

/** \copydoc format_write_message */
void format_write_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes)
{
  cJSON_AddItemToObject(jobjToSend, "clientCommandCode", cJSON_CreateNumber(WRITE_CMD_CODE)); 
  cJSON_AddItemToObject(jobjToSend, "offset", cJSON_CreateNumber(offset)); 
  cJSON_AddItemToObject(jobjToSend, "nBytes", cJSON_CreateNumber(nBytes)); 
  cJSON_AddItemToObject(jobjToSend, "rfaPath", cJSON_CreateString(filepath)); 
}

// /** \copydoc format_monitor_message */
// void format_monitor_message(cJSON *jobjToSend, char *filepath)
// {
//   cJSON_AddItemToObject(jobjToSend, "requestCode", cJSON_CreateNumber(commandCode)); /*Add command request code to JSON object*/
//   cJSON_AddItemToObject(jobjToSend, "username", cJSON_CreateString(username));       /*Add username to JSON object*/
// }