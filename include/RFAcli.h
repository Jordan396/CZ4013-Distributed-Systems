/****************************************************************************
 * @author:                                                                 *
 * - Jordan396 <https://github.com/Jordan396/>                              *
 * - leechongyan <https://github.com/leechongyan/>                          *
 * - seaerchin <https://github.com/seaerchin/>                              *
 *                                                                          *
 *                                                                          *
 ****************************************************************************/

/**
 * @file ttweet_common.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2019
 * @brief Documentation for functions in ttweet_common.h.
 *
 * This header file has been created to xxx
 *
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#pragma once
/* SERVER STATUS CODES */
// Check with Jia Chin

/* Standard libraries */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <chrono>
#include <cstring>
#include <ctype.h> /* for char validation */
#include <fstream>
#include <iostream>     // For cout and cerr
#include <signal.h>     /* for sigaction() */
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <sys/mman.h>   /* to create shared memory across child processes */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <sys/wait.h>   /* for waitpid() */
#include <thread>
#include <time.h>   /* for waitFor() */
#include <unistd.h> /* for close() */

/* External libraries */
#include "FileHandler.h"
#include "Global.h"
#include "cJSON.h" // For message formatting

/* CLIENT COMMAND CODES */
#define FETCH_LAST_MODIFIED_TIME_CMD 0
#define READ_CMD 1
#define WRITE_CMD 2
#define REGISTER_CMD 3
#define CLEAR_FILE_CMD 4

/* SERVER STATUS CODES */
// defined in server class; can shift here if needed
#define FETCH_LAST_MODIFIED_TIME_SUCCESS 100
#define FETCH_LAST_MODIFIED_TIME_FAILURE 101
#define READ_SUCCESS 110
#define READ_FAILURE 111
#define WRITE_SUCCESS 120
#define WRITE_FAILURE 121
#define MONITOR_SUCCESS 130
#define MONITOR_FAILURE 131
#define MONITOR_EXPIRED 132
#define MONITOR_UPDATE 133
#define CLEAR_FILE_SUCCESS 140
#define CLEAR_FILE_FAILURE 141

class RFAcli {
public:
  RFAcli();
  int download_file(string remote_filepath, string local_filepath);
  time_t fetch_last_modified_time(string remote_filepath);
  int register_client(string remote_filepath, string local_filepath,
                      string monitor_duration);
  void clear_file(string remote_filepath);
  string receive_message(bool monitorFlag);
  int send_message(string message);
  int get_response_code(cJSON *jobjReceived);
  int get_nBytes(cJSON *jobjReceived);
  void write_file(string remote_filepath, string toWrite, int nOffset);
  time_t get_last_modified_time(cJSON *jobjReceived);
  string non_blocking_send_receive(string request);
  string get_content(cJSON *jobjReceived);
  int get_response_id(cJSON *jobjReceived);
  string blocking_receive(int monitor_duration);
  void reset_destAddr();
  ~RFAcli();

private:
  FileHandler fh;
  int inboundSockFD, outboundSockFD;
  int monitorDuration;
  time_t registerTime;
  sockaddr_in destAddr = {0};   // Address and port of server
  sockaddr_in sourceAddr = {0}; // Address and port of client
  void init_socket(bool monitorFlag);
  void display_progress(int monitor_duration);
};