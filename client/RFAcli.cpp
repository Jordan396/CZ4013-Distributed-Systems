  
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

// TODO: Integrate the hardcoded variables below with command prompt parser 
// Variables are used to test read/write file access on server
char *filepathHardcode = "RFA://Documents/Cinderella.txt"; // path to file on server
int offsetHardcode = 0;  // offset to start R/W from
int nBytesHardcode = 20; // number of bytes to R/W
// Variables relating to server addressing
char *servAddressHardcode = "172.21.148.168";
unsigned short servPortHardcode = Socket::resolveService("2222", "udp");

/* Variables to handle transfer of data */
const int BUFFER_SIZE = 255;     // Longest string to echo
char clientBuffer[BUFFER_SIZE]; /* String response received */
std::string request;        /* String response received */
std::string response;

// Interface for Chong Yan's reference
/**
 * @brief Fetch file from remote to cache in local.
 *
 * @param remote_filepath Filepath of file in server. (e.g. "RFA://Documents/Cinderella.txt")
 * @param local_filepath Filepath to upload file to. (e.g. ""../client/CacheManager/TempFiles/Cinderalla.txt")
 */
void fetch_file(string remote_filepath, string local_filepath);
/**
 * @brief Fetch file from remote to cache in local.
 *
 * @param remote_filepath Filepath of file in server. (e.g. "RFA://Documents/Cinderella.txt")
 * @param local_filepath Filepath to upload file to. (e.g. ""../client/CacheManager/TempFiles/Cinderalla.txt")
 */
void fetch_file(string remote_filepath, string local_filepath);
/**
 * @brief Writes to the remote file.
 *
 * @param remote_filepath Filepath of file in server. (e.g. "RFA://Documents/Cinderella.txt")
 * @param toWrite String to be written
 * @param nOffset Offset in file to write at
 */
void write_file(string remote_filepath, string toWrite, int nOffset);


/* Function declarations */
// void format_read_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes);
// void format_write_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes);

int receive_message(){
  try {
    UDPSocket sock(servPortHardcode);                
  
    int recvMsgSize;                  // Size of received message
    string sourceAddress;             // Address of datagram source
    unsigned short sourcePort;        // Port of datagram source

    // Block until receive message from a client
    cout << "Listening..." << endl;
    recvMsgSize = sock.recvFrom(clientBuffer, BUFFER_SIZE, sourceAddress, sourcePort);
    cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;
    strncpy(response, clientBuffer, sizeof(clientBuffer));

    // Process response

  }
  catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  return 0;
}

int send_message(string destAddress, unsigned short destPort, string message){
  try {
    UDPSocket sock;
    sock.sendTo(message, strlen(message), destAddress, destPort);

  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  return 0;
}

// void build_get_file_reference_message(cJSON *jobjToSend, char *filepath){
//   cJSON_AddItemToObject(jobjToSend, "clientCommandCode", cJSON_CreateNumber(READ_CMD_CODE)); 
//   cJSON_AddItemToObject(jobjToSend, "rfaPath", cJSON_CreateString(filepath)); 
// }

// /** \copydoc format_read_message */
// void format_read_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes)
// {
//   cJSON_AddItemToObject(jobjToSend, "clientCommandCode", cJSON_CreateNumber(READ_CMD_CODE)); 
//   cJSON_AddItemToObject(jobjToSend, "offset", cJSON_CreateNumber(offset)); 
//   cJSON_AddItemToObject(jobjToSend, "nBytes", cJSON_CreateNumber(nBytes)); 
//   cJSON_AddItemToObject(jobjToSend, "rfaPath", cJSON_CreateString(filepath)); 
// }

// /** \copydoc format_write_message */
// void format_write_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes)
// {
//   cJSON_AddItemToObject(jobjToSend, "clientCommandCode", cJSON_CreateNumber(WRITE_CMD_CODE)); 
//   cJSON_AddItemToObject(jobjToSend, "offset", cJSON_CreateNumber(offset)); 
//   cJSON_AddItemToObject(jobjToSend, "nBytes", cJSON_CreateNumber(nBytes)); 
//   cJSON_AddItemToObject(jobjToSend, "rfaPath", cJSON_CreateString(filepath)); 
// }

// /** \copydoc format_monitor_message */
// void format_monitor_message(cJSON *jobjToSend, char *filepath)
// {
//   cJSON_AddItemToObject(jobjToSend, "requestCode", cJSON_CreateNumber(commandCode)); /*Add command request code to JSON object*/
//   cJSON_AddItemToObject(jobjToSend, "username", cJSON_CreateString(username));       /*Add username to JSON object*/
// }