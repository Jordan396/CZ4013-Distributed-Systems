
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
// test git
/**
 * @file client.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date xxx 2020
 * @brief .
 *
 *
 */

#include "RFAcli.h"

// /**
//  * @brief Download file from remote to cache in local.
//  *
//  * @param remote_filepath Filepath of file in server. (e.g.
//  "RFA://Documents/Cinderella.txt")
//  * @param local_filepath Filepath to upload file to. (e.g.
//  ""../client/CacheManager/TempFiles/Cinderalla.txt")
//  * @return int 1 if download successful, 0 otherwise.
//  */
// int download_file(string remote_filepath, string local_filepath);
// /**
//  * @brief Fetch last modified time from remote to cache in local.
//  *
//  * @param remote_filepath Filepath of file in server. (e.g.
//  "RFA://Documents/Cinderella.txt")
//  * @param last_modified_time Last modified time
//  * @return int 1 if operation successful, 0 otherwise.
//  */
// int fetch_last_modified_time(string remote_filepath, string
// last_modified_time);
// /**
//  * @brief Register the client for monitoring a file over xxx time.
//  *
//  * @param remote_filepath Filepath of file in server. (e.g.
//  "RFA://Documents/Cinderella.txt")
//  * @param monitor_interval Time to monitor for
//  */
// int register_client(string remote_filepath, string monitor_interval);
// /**
//  * @brief Writes to the remote file.
//  *
//  * @param remote_filepath Filepath of file in server. (e.g.
//  "RFA://Documents/Cinderella.txt")
//  * @param toWrite String to be written
//  * @param nOffset Offset in file to write at
//  */
// void write_file(string remote_filepath, string toWrite, int nOffset);

RFAcli::RFAcli(void) {
  // Creating socket file descriptor
  if ((inboundSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  if ((outboundSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&destAddr, 0, sizeof(destAddr));
  memset(&sourceAddr, 0, sizeof(sourceAddr));

  // Filling source information
  sourceAddr.sin_family = AF_INET; // IPv4
  sourceAddr.sin_addr.s_addr = INADDR_ANY;
  sourceAddr.sin_port =
      htons((unsigned short)strtoul(clientPortNo.c_str(), NULL, 0));
  // Filling destination information
  destAddr.sin_family = AF_INET; // IPv4
  destAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
  destAddr.sin_port =
      htons((unsigned short)strtoul(serverPortNo.c_str(), NULL, 0));

  init_socket(false);

  // Bind the socket with the source address
  socklen_t len = sizeof(sourceAddr);
  if (bind(inboundSockFD, (const struct sockaddr *)&sourceAddr, len) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (getsockname(inboundSockFD, (struct sockaddr *)&sourceAddr, &len) == -1) {
    perror("getsockname");
  } else {
    clientPortNo = std::to_string(ntohs(sourceAddr.sin_port));
    cout << "Source port number: " + clientPortNo << endl;
  }
}

RFAcli::~RFAcli() {}


void RFAcli::init_socket(bool monitorFlag) {
    // set timeout
    struct timeval timeout_val;
    if (monitorFlag) {
        timeout_val.tv_sec = monitorDuration;
        timeout_val.tv_usec = 0;
    }
    else {
        timeout_val.tv_sec = (int)timeOut/1000;
        timeout_val.tv_usec = (int)((timeOut%1000)*1000);
    }

    // timeout_val.tv_usec = timeOut * 1000;
    if (setsockopt(inboundSockFD, SOL_SOCKET, SO_RCVTIMEO, &timeout_val,
        sizeof(timeout_val)) < 0) {
        perror("failed to set timeout");
        exit(EXIT_FAILURE);
    }
}
/****************************************************************************
 *                                                                          *
 *                    Network Methods                                       *
 *                                                                          *
 ****************************************************************************/
string RFAcli::receive_message(bool monitorFlag) {
  cout << "Listening..." << endl;
  char clientBuffer[udpDatagramSize + 1];
  if (monitorFlag) {
      cout << "left how much time: " << monitorDuration << endl;
      if (monitorDuration == 0) return "";
      init_socket(true);
  }
  int len = sizeof(destAddr);
  int n = recvfrom(inboundSockFD, clientBuffer, udpDatagramSize, MSG_WAITALL,
                   (struct sockaddr *)&destAddr, (socklen_t *)&len);
  if (monitorFlag) {
      cout << "left how much time: " << monitorDuration << endl;
      init_socket(false);
  }
  if (n == -1) {
    return "";
  }
  clientBuffer[n] = '\0';

  string sourceAddress;      // Address of datagram source
  unsigned short sourcePort; // Port of datagram source
  sourceAddress = inet_ntoa(destAddr.sin_addr);
  sourcePort = ntohs(destAddr.sin_port);
  cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;

  // Reset destAddr
  reset_destAddr();

  // cout << "Received packet to " << sourceAddress << ":" << sourcePort <<
  // endl;
  string s = clientBuffer;
  clientBuffer[0] = '\0';
  cout << "Received message:\n" + s << endl;

  return s;
}

int RFAcli::send_message(string message) {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // Reset destAddr
  reset_destAddr();

  sendto(outboundSockFD, message.c_str(), strlen(message.c_str()), 0,
         (const struct sockaddr *)&destAddr, sizeof(destAddr));
  cout << "Sending message: " + message + " : to " +
              (char *)inet_ntoa((struct in_addr)destAddr.sin_addr)
       << endl;
  return 0;
}

void RFAcli::reset_destAddr() {
  // Filling destination information
  destAddr.sin_family = AF_INET; // IPv4
  destAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
  destAddr.sin_port =
      htons((unsigned short)strtoul(serverPortNo.c_str(), NULL, 0));
}

string RFAcli::non_blocking_send_receive(string request) {
  int request_identifier;
  int response_identifier;
  string response;

  // Assign identifier
  request_identifier = request.size();

  // Prepare to receive response
  cJSON *jobjReceived;
  jobjReceived = cJSON_CreateObject();

  while (true) {
    // Send messege
    send_message(request);

    // Receive response
    response = receive_message(false);
    while (response.compare("") != 0) { // messege at socket
      jobjReceived = cJSON_Parse(response.c_str());
      response_identifier = get_response_id(jobjReceived);
      if (request_identifier ==
          response_identifier) { // correct message received
        cJSON_Delete(jobjReceived);
        return response;
      }
      response = receive_message(false);
    }
  }
}

string RFAcli::blocking_receive(int monitor_duration) {
  // Wait for response...
  string response;
  int time_left = monitorDuration - (int)difftime(time(0), registerTime);
  time_left = max(time_left, 0);
  monitorDuration = time_left;
  display_progress(monitor_duration);
  response = receive_message(true);
  
  return response;
}

void RFAcli::display_progress(int monitor_duration) {
    double progress = (double)(monitor_duration-monitorDuration)/(double)(monitor_duration);
    int barWidth = 50;
    if (monitor_duration == -1) progress = 1.0;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();

    std::cout << std::endl;
}


/****************************************************************************
 *                                                                          *
 *                    Program Features                                      *
 *                                                                          *
 ****************************************************************************/

int RFAcli::download_file(string remote_filepath, string local_filepath) {
  int offset = 0;
  int nBytes;
  string content;
  string response;

  while (true) { // read all bytes
    cJSON *jobjToSend;
    jobjToSend = cJSON_CreateObject();
    cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE",
                          cJSON_CreateNumber(READ_CMD));
    cJSON_AddItemToObject(jobjToSend, "RFA_PATH",
                          cJSON_CreateString(remote_filepath.c_str()));
    cJSON_AddItemToObject(jobjToSend, "N_BYTES",
                          cJSON_CreateNumber(bufferSize));
    cJSON_AddItemToObject(jobjToSend, "OFFSET", cJSON_CreateNumber(offset));
    cJSON_AddItemToObject(jobjToSend, "REQUEST_ID", cJSON_CreateNumber(rand()));
    cJSON_AddItemToObject(jobjToSend, "PORT",
                          cJSON_CreateString(clientPortNo.c_str()));
    response = non_blocking_send_receive(cJSON_Print(jobjToSend));
    cJSON_Delete(jobjToSend);

    // Parse response message
    cJSON *jobjReceived;
    jobjReceived = cJSON_CreateObject();
    jobjReceived = cJSON_Parse(response.c_str());
    content = get_content(jobjReceived);

    if (get_response_code(jobjReceived) == READ_SUCCESS) {
      // Create empty file
      if (offset == 0) {
        fh.CreateFile(local_filepath.c_str());
      }

      // Read nBytes returned
      nBytes = get_nBytes(jobjReceived);

      // EOF reached
      if (nBytes == 0) {
        break;
      }

      // Write content to local
      cout << "Writing to local path: " + local_filepath << endl;
      fh.WriteFile(local_filepath.c_str(), content.c_str(), offset);
      // Increase offset
      offset += nBytes;
      cJSON_Delete(jobjReceived);
    } else {
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
time_t RFAcli::fetch_last_modified_time(string remote_filepath) {
  // Response
  string response; // blocking here.
  time_t last_modified_time;

  // Send request
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE",
                        cJSON_CreateNumber(FETCH_LAST_MODIFIED_TIME_CMD));
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH",
                        cJSON_CreateString(remote_filepath.c_str()));
  cJSON_AddItemToObject(jobjToSend, "PORT",
                        cJSON_CreateString(clientPortNo.c_str()));
  cJSON_AddItemToObject(jobjToSend, "REQUEST_ID", cJSON_CreateNumber(rand()));
  response = non_blocking_send_receive(cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);

  // Parse response message
  cJSON *jobjReceived;
  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(response.c_str());
  if (get_response_code(jobjReceived) == FETCH_LAST_MODIFIED_TIME_SUCCESS) {
    last_modified_time = get_last_modified_time(jobjReceived);
    cJSON_Delete(jobjReceived);
    return last_modified_time;
  }
  cJSON_Delete(jobjReceived);
  cout << "Unable to retrieve last modified time." << endl;
  return last_modified_time;
}

void RFAcli::write_file(string remote_filepath, string toWrite, int nOffset) {
  // Response
  std::string response;

  // Send request
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE",
                        cJSON_CreateNumber(WRITE_CMD));
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH",
                        cJSON_CreateString(remote_filepath.c_str()));
  cJSON_AddItemToObject(jobjToSend, "CONTENT",
                        cJSON_CreateString(toWrite.c_str()));
  cJSON_AddItemToObject(jobjToSend, "OFFSET", cJSON_CreateNumber(nOffset));
  cJSON_AddItemToObject(jobjToSend, "PORT",
                        cJSON_CreateString(clientPortNo.c_str()));
  cJSON_AddItemToObject(jobjToSend, "REQUEST_ID", cJSON_CreateNumber(rand()));
  response = non_blocking_send_receive(cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);

  // Parse response message
  cJSON *jobjReceived;
  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(response.c_str());
  if (get_response_code(jobjReceived) == WRITE_SUCCESS) {
    cJSON_Delete(jobjReceived);
    return;
  }
  if (get_response_code(jobjReceived) == WRITE_FAILURE) {
    cout << "ERROR: Clear file operation failed." << endl;
    cJSON_Delete(jobjReceived);
    return;
  }
  cout << "ERROR: Unknown response received." << endl;
  cJSON_Delete(jobjReceived);
  return;
}

int RFAcli::register_client(string remote_filepath, string local_filepath,
                            string monitor_duration) {
  // Response
  std::string response;
  registerTime = time(0);
  monitorDuration = stoi(monitor_duration);
  // Send request
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE",
                        cJSON_CreateNumber(REGISTER_CMD));
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH",
                        cJSON_CreateString(remote_filepath.c_str()));
  cJSON_AddItemToObject(jobjToSend, "MONITOR_DURATION",
                        cJSON_CreateString(monitor_duration.c_str()));
  cJSON_AddItemToObject(jobjToSend, "PORT",
                        cJSON_CreateString(clientPortNo.c_str()));
  cJSON_AddItemToObject(jobjToSend, "REQUEST_ID", cJSON_CreateNumber(rand()));
  response = non_blocking_send_receive(cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);

  // Parse response message
  cJSON *jobjReceived;
  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(response.c_str());

  // Failed to register
  if (get_response_code(jobjReceived) == MONITOR_FAILURE) {
      cout << "LOGS: Monitor failed." << endl;
    cJSON_Delete(jobjReceived);
    return 0;
  }
  // Successfully registered
  if (get_response_code(jobjReceived) == MONITOR_SUCCESS) {
      cout << "LOGS: Monitor success." << endl;
    cJSON_Delete(jobjReceived);

    // Enter monitoring loop...
    while (true) {
      response = blocking_receive(stoi(monitor_duration));

      // Parse response message
      cJSON *jobjReceived;
      jobjReceived = cJSON_CreateObject();
      jobjReceived = cJSON_Parse(response.c_str());
      if (response == "") break;
      // monitor duration expired
      if (get_response_code(jobjReceived) == MONITOR_EXPIRED) {
          cout << "LOGS: Monitor expired." << endl;
        cJSON_Delete(jobjReceived);
        return 1;
      } else if (get_response_code(jobjReceived) ==
                 MONITOR_UPDATE) { // changes encountered
          cout << "LOGS: Changes detected. Redownloading file..." << endl;
        cJSON_Delete(jobjReceived);
        // Redownload file
        if (download_file(remote_filepath, local_filepath) != 1) {
          cout << "ERROR: Cannot download file. Exiting...";
          return 0;
        }
      } else {
        cJSON_Delete(jobjReceived);
        cout << "LOGS: Unknown response received.";
      }
    }
  }
  display_progress(-1);
  cJSON_Delete(jobjReceived);
  cout << "LOGS: Unknown response received.";
  return 1;
}

void RFAcli::clear_file(string remote_filepath) {
  // Response
  std::string response;

  // Send request
  cJSON *jobjToSend;
  jobjToSend = cJSON_CreateObject();
  cJSON_AddItemToObject(jobjToSend, "REQUEST_CODE",
                        cJSON_CreateNumber(CLEAR_FILE_CMD));
  cJSON_AddItemToObject(jobjToSend, "REQUEST_ID", cJSON_CreateNumber(rand()));
  cJSON_AddItemToObject(jobjToSend, "RFA_PATH",
                        cJSON_CreateString(remote_filepath.c_str()));
  cJSON_AddItemToObject(jobjToSend, "PORT",
                        cJSON_CreateString(clientPortNo.c_str()));
  cJSON_AddItemToObject(jobjToSend, "REQUEST_ID", cJSON_CreateNumber(rand()));
  response = non_blocking_send_receive(cJSON_Print(jobjToSend));
  cJSON_Delete(jobjToSend);

  // Parse response message
  cJSON *jobjReceived;
  jobjReceived = cJSON_CreateObject();
  jobjReceived = cJSON_Parse(response.c_str());
  if (get_response_code(jobjReceived) == CLEAR_FILE_SUCCESS) {
    cJSON_Delete(jobjReceived);
    return;
  }
  if (get_response_code(jobjReceived) == CLEAR_FILE_FAILURE) {
    cout << "ERROR: Clear file operation failed." << endl;
    cJSON_Delete(jobjReceived);
    return;
  }
  cout << "ERROR: Unknown response received." << endl;
  cJSON_Delete(jobjReceived);
  return;
}

/****************************************************************************
 *                                                                          *
 *                    Getter methods to parse response                      *
 *                                                                          *
 ****************************************************************************/

int RFAcli::get_response_code(cJSON *jobjReceived) {
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "RESPONSE_CODE")
      ->valueint;
}

time_t RFAcli::get_last_modified_time(cJSON *jobjReceived) {
  struct tm tm;
  string time_details =
      cJSON_GetObjectItemCaseSensitive(jobjReceived, "LAST_MODIFIED")
          ->valuestring;
  strptime(time_details.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
  return mktime(&tm);
}

int RFAcli::get_nBytes(cJSON *jobjReceived) {
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "N_BYTES")->valueint;
}

string RFAcli::get_content(cJSON *jobjReceived) {
  return cJSON_GetObjectItem(jobjReceived, "CONTENT")->valuestring;
}

int RFAcli::get_response_id(cJSON *jobjReceived) {
  return cJSON_GetObjectItemCaseSensitive(jobjReceived, "RESPONSE_ID")
      ->valueint;
}
