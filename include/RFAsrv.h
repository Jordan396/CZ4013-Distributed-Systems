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


/* CLIENT COMMAND CODES */
#define GET_LAST_MODIFIED_TIME_CMD 0
#define READ_CMD 1
#define WRITE_CMD 2  
#define REGISTER_CMD 3 

/* SERVER STATUS CODES */ 
// defined in server class; can shift here if needed

/* Standard libraries */
#define _GNU_SOURCE
#include <stdio.h>      /* for printf() and fprintf() */
#include <iostream>          // For cout and cerr
#include <fstream>
#include <pthread.h>
#include <thread>
#include <chrono>
#include <ctime> 
#include <time.h>  
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <cstring>
#include <map>
#include <string>
#include <functional>
#include <list> 
#include <stack>
#include <iterator> 
#include <unistd.h>     /* for close() */
#include <signal.h>     /* for sigaction() */
#include <ctype.h>      /* for char validation */
#include <time.h>       /* for waitFor() */
#include <sys/mman.h>   /* to create shared memory across child processes */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <sys/wait.h>   /* for waitpid() */
#include <sys/stat.h>
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */


#include "./Global.h"
#include "./FileHandler.h"
/* External libraries */
#include "./RFAsockets.h"      // For UDPSocket and SocketException
#include "./cJSON.h"      // For message formatting

void *monitor_registered_clients( void *ptr );
int comparetime(time_t time1,time_t time2);
int send_message(string destAddress, unsigned short destPort, string message);
void process_request(string sourceAddress, unsigned short sourcePort, string request);
void execute_get_last_modified_time_command(string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived);
void execute_read_command(string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived);
void execute_write_command(string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived);
void execute_register_command(string sourceAddress, unsigned short sourcePort, cJSON *jobjReceived);
void update_registered_clients(string filepath);
bool is_request_exists(string sourceAddress, unsigned short sourcePort, string message);
void store_request(string sourceAddress, unsigned short sourcePort, string message);
void store_response(string sourceAddress, unsigned short sourcePort, string message);
void retrieve_response(string sourceAddress, unsigned short sourcePort, string message);
int get_request_code(cJSON *jobjReceived);
int get_offset(cJSON *jobjReceived);
int get_nBytes(cJSON *jobjReceived);
void get_filepath(cJSON *jobjReceived, string filepath);
void get_monitor_duration(cJSON *jobjReceived, string monitor_duration);
char* get_toWrite(cJSON *jobjReceived);
bool translate_filepath(string pseudo_filepath, string actual_filepath);
void get_last_modified_time(const char *path, string last_modified_time);


// /**
//  * @brief Accepts a cJSON object and sends its string representation over a socket.
//  *
//  * This function converts a cJSON object to its string representation.
//  * It then sends this string to the other party on the network.
//  * 
//  * This payload adopts the following structure:
//  * The first RCV_BUF_SIZE bytes indicates the size of the actual payload.
//  * The remaining bytes contain the actual cJSON string representation payload.
//  *
//  * @param sock Client socket assigned to the connection.
//  * @param jobjToSend cJSON object to be sent.
//  * @return int 0 if error occurred, 1 otherwise.
//  */
// int send_message(int sock, cJSON *jobjToSend);

// /**
//  * @brief Receives a send_payload formatted response and saves it to objReceived.
//  *
//  * The socket listens for a send_payload formatted response.
//  * It then saves the response to an objReceived string.
//  * 
//  * This reponse adopts the following structure:
//  * The first RCV_BUF_SIZE bytes indicates the size of the actual payload.
//  * The remaining bytes contain the actual cJSON string representation payload.
//  *
//  * @param sock Client socket assigned to the connection.
//  * @param objReceived String to save the response recieved.
//  * @return void
//  */
// void receive_message(int sock, char *objReceived);

// /**
//  * @brief Waits for secs amount of seconds.
//  *
//  * @param secs Number of seconds to wait for.
//  * @return void
//  */
// void wait_for(unsigned int secs);