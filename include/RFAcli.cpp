  
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

#include "RFAcli.h"

// Interface for Chong Yan's reference
// /**
//  * @brief Download file from remote to cache in local.
//  *
//  * @param remote_filepath Filepath of file in server. (e.g. "RFA://Documents/Cinderella.txt")
//  * @param local_filepath Filepath to upload file to. (e.g. ""../client/CacheManager/TempFiles/Cinderalla.txt")
//  * @return int 1 if download successful, 0 otherwise.
//  */
// int download_file(string remote_filepath, string local_filepath);
// /**
//  * @brief Fetch last modified time from remote to cache in local.
//  *
//  * @param remote_filepath Filepath of file in server. (e.g. "RFA://Documents/Cinderella.txt")
//  * @param last_modified_time Last modified time
//  * @return int 1 if operation successful, 0 otherwise.
//  */
// int get_last_modified_time(string remote_filepath, string last_modified_time);
// /**
//  * @brief Register the client for monitoring a file over xxx time.
//  *
//  * @param remote_filepath Filepath of file in server. (e.g. "RFA://Documents/Cinderella.txt")
//  * @param monitor_interval Time to monitor for
//  */
// int register_client(string remote_filepath, string monitor_interval);
// /**
//  * @brief Writes to the remote file.
//  *
//  * @param remote_filepath Filepath of file in server. (e.g. "RFA://Documents/Cinderella.txt")
//  * @param toWrite String to be written
//  * @param nOffset Offset in file to write at
//  */
// void write_file(string remote_filepath, string toWrite, int nOffset);


/* Function declarations */
// void format_read_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes);
// void format_write_message(cJSON *jobjToSend, char *filepath, int offset, int nBytes);


int RFAcli::download_file(string remote_filepath, string local_filepath){
  string response;
  int offset = 0;

  while(true){
    // Send request
    cJSON *jobjToSend;
    jobjToSend = cJSON_CreateObject();
    cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE", cJSON_CreateNumber(READ_CMD)); 
    cJSON_AddItemToObject(jobjToSend, "N_BYTES", cJSON_CreateNumber(BUFFER_SIZE)); 
    cJSON_AddItemToObject(jobjToSend, "OFFSET", cJSON_CreateNumber(offset)); 
    send_message(serverIP, portNo, cJSON_Print(jobjToSend));
    cJSON_Delete(jobjToSend);

    // Wait for response...
    receive_message(response);

    // Parse response message
    cJSON *jobjReceived;
    jobjReceived = cJSON_CreateObject();
    jobjReceived = cJSON_Parse(response.c_str());
    if (get_response_code(jobjReceived) == 1){
      // write();
      // Increase offset
      offset += BUFFER_SIZE;
      cJSON_Delete(jobjReceived);
    }
    else {
      cout << "ERROR: File cannot be read." << endl;
      cJSON_Delete(jobjReceived);
      return 0;
    }
  }
  return 1;
}

int RFAcli::get_last_modified_time(string remote_filepath, string last_modified_time){
  // Send request
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE", cJSON_CreateNumber(GET_LAST_MODIFIED_TIME_CMD)); 
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH", cJSON_CreateString(remote_filepath.c_str())); 
  send_message(serverIP, portNo, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);

  // Wait for response...
  string response;
  receive_message(response);

  // Parse response message
  cJSON *jobjReceived;
  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(response.c_str());
  if (get_response_code(jobjReceived) == 1){
    extract_last_modified_time(jobjReceived, last_modified_time);
    cJSON_Delete(jobjReceived);
    return 1;
  }
  cJSON_Delete(jobjReceived);
  return 0;
}

int RFAcli::register_client(string remote_filepath, string monitor_duration){
  // Response
  std::string response;

  // Send request
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE", cJSON_CreateNumber(REGISTER_CMD)); 
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH", cJSON_CreateString(remote_filepath.c_str()));
  cJSON_AddItemToObject(jobjToSend, "MONITOR_DURATION", cJSON_CreateString(monitor_duration.c_str()));  
  send_message(serverIP, portNo, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);

  // Wait for response...
  receive_message(response);

  // Parse response message
  cJSON *jobjReceived;
  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(response.c_str());
  if (get_response_code(jobjReceived) == 0){
    cJSON_Delete(jobjReceived);
    return 0;
  }
  cJSON_Delete(jobjReceived);

  // Enter monitoring loop...
  while (true){
    // Wait for response...
    receive_message(response);

    // Parse response message
    cJSON *jobjReceived;
    jobjReceived = cJSON_CreateObject();
    jobjReceived = cJSON_Parse(response.c_str());
    if (get_response_code(jobjReceived) == 1000){ // monitor duration expired
      cJSON_Delete(jobjReceived);
      break;
    }
    else { // file written to, proceed to read new file
      cJSON_Delete(jobjReceived);
      if (download_file(remote_filepath, local_filepath) != 1){
        cout << "ERROR: Cannot download file. Exiting...";
        return 0;
      }
    }
  }
  return 1;
}

int RFAcli::receive_message(string response){
  try {
    UDPSocket sock(portNo);                
  
    int recvMsgSize;                  // Size of received message
    string serverIP;             // Address of datagram source
    unsigned short portNo;        // Port of datagram source


    // Block until receive message from a client
    cout << "Listening..." << endl;
    recvMsgSize = sock.recvFrom(clientBuffer, BUFFER_SIZE, serverIP, portNo);
    cout << "Received packet from " << serverIP << ":" << portNo << endl;
    strncpy(response, clientBuffer, sizeof(clientBuffer));
  }
  catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  return 0;
}

int RFAcli::send_message(string destAddress, string destPort, string message){
  try {
    UDPSocket sock;
    unsigned short destPort = (unsigned short) strtoul(destPort, NULL, 0);
    sock.sendTo(message, strlen(message), destAddress, destPort);

  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  return 0;
}

int RFAcli::get_response_code(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "RESPONSE_CODE")->valueint;
}

void RFAcli::extract_last_modified_time(cJSON *jobjReceived, string last_modified)
{
  strcpy(cJSON_GetObjectItemCaseSensitive(jobjReceived, "LAST_MODIFIED")->valuestring, last_modified);
}

void RFAcli::write_file(string remote_filepath, string toWrite, int nOffset){
  // Response
  std::string response;

  // Send request
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE", cJSON_CreateNumber(WRITE_CMD)); 
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH", cJSON_CreateString(remote_filepath));
  send_message(serverIP, portNo, cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);

  // // Wait for response...
  // receive_message(response);

  // // Parse response message
  // cJSON *jobjReceived;
  // jobjReceived = cJSON_CreateObject();
  // jobjReceived = cJSON_Parse(response);
  // if (get_response_code(response) == 0){
  //   cJSON_Delete(jobjReceived);
  //   return 0;
  // }
  // cJSON_Delete(jobjReceived);
}

RFAcli::~RFAcli()
{
}
