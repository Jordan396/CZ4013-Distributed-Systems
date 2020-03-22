  
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

RFAcli::RFAcli(void){
  // Creating socket file descriptor 
  if ( (inboundSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
      perror("socket creation failed"); 
      exit(EXIT_FAILURE); 
  } 
  if ( (outboundSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
      perror("socket creation failed"); 
      exit(EXIT_FAILURE); 
  } 

  memset(&destAddr, 0, sizeof(destAddr)); 
  memset(&sourceAddr, 0, sizeof(sourceAddr)); 

  // Filling source information 
  sourceAddr.sin_family    = AF_INET; // IPv4 
  sourceAddr.sin_addr.s_addr = INADDR_ANY; 
  sourceAddr.sin_port = htons((unsigned short) strtoul(clientPortNo.c_str(), NULL, 0)); 
  // Filling destination information 
  destAddr.sin_family    = AF_INET; // IPv4 
  destAddr.sin_addr.s_addr = inet_addr(serverIP.c_str()); 
  destAddr.sin_port = htons((unsigned short) strtoul(serverPortNo.c_str(), NULL, 0));

  // Bind the socket with the source address 
  socklen_t len = sizeof(sourceAddr);
  if (bind(inboundSockFD, (const struct sockaddr *)&sourceAddr, len) < 0 ) 
  { 
      perror("bind failed"); 
      exit(EXIT_FAILURE); 
  }
  if (getsockname(inboundSockFD, (struct sockaddr *)&sourceAddr, &len) == -1){
    perror("getsockname");
  }
  else {
    clientPortNo = std::to_string(ntohs(sourceAddr.sin_port));
    cout << "Source port number: " + clientPortNo << endl;
  }
}


int RFAcli::download_file(string remote_filepath, string local_filepath){
  string response;
  int offset = 0;
  FileHandler fh = FileHandler(); 

  while(true){
    // Send request
    cJSON *jobjToSend;
    jobjToSend = cJSON_CreateObject();
    cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE", cJSON_CreateNumber(READ_CMD)); 
    cJSON_AddItemToObject(jobjToSend, "N_BYTES", cJSON_CreateNumber(bufferSize)); 
    cJSON_AddItemToObject(jobjToSend, "OFFSET", cJSON_CreateNumber(offset)); 
    cJSON_AddItemToObject(jobjToSend, "PORT", cJSON_CreateString(clientPortNo.c_str())); 
    send_message(cJSON_Print(jobjToSend));
    cout << cJSON_Print(jobjToSend) << endl; 
    cJSON_Delete(jobjToSend);

    // Wait for response...
    response = receive_message();

    // Parse response message
    cJSON *jobjReceived;
    jobjReceived = cJSON_CreateObject();
    jobjReceived = cJSON_Parse(response.c_str());
    if (get_response_code(jobjReceived) == 1){
      cout << "writing" << endl; 
      fh.WriteFile(local_filepath.c_str(), cJSON_GetObjectItem(jobjReceived, "CONTENT")->valuestring, offset);
      // Increase offset
      offset += bufferSize;
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


/**
 * get last modified time of file on server
 * @param remote_filepath filepath that we want to query
 * @param last_modified_time local last modified time of requested file 
 * @return last modified time, caller checks for empty string 
 **/
string RFAcli::get_last_modified_time(string remote_filepath){
  // Send request
  // TODO: jordan debug this please
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE", cJSON_CreateNumber(GET_LAST_MODIFIED_TIME_CMD)); 
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH", cJSON_CreateString(remote_filepath.c_str())); 
  cJSON_AddItemToObject(jobjToSend, "PORT", cJSON_CreateString(clientPortNo.c_str())); 
  send_message(cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);
  // Wait for response...
  string response; // blocking here.
  response = receive_message();
  cout << "response is: " << response << endl; 
  // Parse response message
  cJSON *jobjReceived;
  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(response.c_str());
  if (get_response_code(jobjReceived) == 1){
    string last_modified_time = extract_last_modified_time(jobjReceived);
    cJSON_Delete(jobjReceived);
    return last_modified_time;
  }
  cJSON_Delete(jobjReceived);
  return "";
}

int RFAcli::register_client(string remote_filepath, string local_filepath, string monitor_duration){
  // Response
  std::string response;

  // Send request
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE", cJSON_CreateNumber(REGISTER_CMD)); 
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH", cJSON_CreateString(remote_filepath.c_str()));
  cJSON_AddItemToObject(jobjToSend, "MONITOR_DURATION", cJSON_CreateString(monitor_duration.c_str()));  
  cJSON_AddItemToObject(jobjToSend, "PORT", cJSON_CreateString(clientPortNo.c_str())); 
  send_message(cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);

  // Wait for response...
  response = receive_message();

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
    response = receive_message();

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

string RFAcli::receive_message(){
  cout << "Listening..." << endl;
  char clientBuffer[bufferSize];

  int len = sizeof(destAddr);
  int n = recvfrom(inboundSockFD, clientBuffer, bufferSize, MSG_WAITALL, ( struct sockaddr *) &destAddr, (socklen_t*)&len); 
  clientBuffer[n] = '\0'; 

  string sourceAddress;             // Address of datagram source
  unsigned short sourcePort;        // Port of datagram source
  sourceAddress = inet_ntoa(destAddr.sin_addr);
  sourcePort = ntohs(destAddr.sin_port);
  cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;

  // cout << "Received packet to " << sourceAddress << ":" << sourcePort << endl;
  string s = clientBuffer;
  return s;
}

int RFAcli::send_message(string message){
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  sendto(outboundSockFD, message.c_str(), strlen(message.c_str()), 0, (const struct sockaddr *) &destAddr, sizeof(destAddr)); 
  cout << "Sending message: " + message + " : to " + (char*)inet_ntoa((struct in_addr)destAddr.sin_addr) << endl;
  return 0;
}


int RFAcli::get_response_code(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "RESPONSE_CODE")->valueint;
}

string RFAcli::extract_last_modified_time(cJSON *jobjReceived)
{
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "LAST_MODIFIED")->valuestring;
}

void RFAcli::write_file(string remote_filepath, string toWrite, int nOffset){
  // Response
  std::string response;

  // Send request
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE", cJSON_CreateNumber(WRITE_CMD)); 
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH", cJSON_CreateString(remote_filepath.c_str()));
  cJSON_AddItemToObject(jobjToSend, "PORT", cJSON_CreateString(clientPortNo.c_str())); 
  send_message(cJSON_Print(jobjToSend));
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
