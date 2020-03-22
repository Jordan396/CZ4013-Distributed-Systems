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
#include <stdio.h>      /* for printf() and fprintf() */
#include <iostream>          // For cout and cerr
#include <fstream>
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <cstring>
#include <unistd.h>     /* for close() */
#include <signal.h>     /* for sigaction() */
#include <ctype.h>      /* for char validation */
#include <time.h>       /* for waitFor() */
#include <chrono>
#include <thread>
#include <sys/mman.h>   /* to create shared memory across child processes */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <sys/wait.h>   /* for waitpid() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */

/* External libraries */
#include "cJSON.h"      // For message formatting
#include "Global.h"
#include "FileHandler.h"

/* CLIENT COMMAND CODES */
#define GET_LAST_MODIFIED_TIME_CMD 0
#define READ_CMD 1
#define WRITE_CMD 2  
#define REGISTER_CMD 3 

class RFAcli 
{
  public:
    RFAcli();
    int download_file(string remote_filepath, string local_filepath);
    string get_last_modified_time(string remote_filepath);
    int register_client(string remote_filepath, string local_filepath, string monitor_duration);
    string receive_message();
    int send_message(string message);
    int get_response_code(cJSON *jobjReceived);
    void write_file(string remote_filepath, string toWrite, int nOffset);
    string extract_last_modified_time(cJSON *jobjReceived);
    ~RFAcli();
  private:
    int inboundSockFD, outboundSockFD;
    sockaddr_in destAddr = {0};
    sockaddr_in sourceAddr = {0}; 
};