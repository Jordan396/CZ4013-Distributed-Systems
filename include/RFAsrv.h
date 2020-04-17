/**
 * @file RFAsrv.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in RFAsrv.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef RFA_SRV_H
#define RFA_SRV_H

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

/* System libraries */
#include <cstring>
#include <string>
#include <string.h> /* for memset() */
#include <ctime>
#include <ctype.h> /* for char validation */
#include <fstream>
#include <functional>
#include <iostream> // For cout and cerr
#include <iterator>
#include <list>
#include <map>
#include <signal.h> /* for sigaction() */
#include <stack>
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <sys/mman.h>   /* to create shared memory across child processes */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <sys/stat.h>
#include <sys/wait.h>  /* for waitpid() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <unistd.h>    /* for close() */
#include <experimental/filesystem>
#include <pthread.h>
#include <thread>
#include <chrono>
#include <time.h>

/* Program libraries */
#include "FileHandler.h"
#include "Global.h"
#include "cJSON.h" // For message formatting
#include "utils.h"

/* RegisteredClient structure */
struct RegisteredClient
{
    string address;
    string port;
    time_t expirationTime;
    time_t registerTime;
} RegisteredClient;

/* 
Network related methods 
*/

/**
 * @brief Initializes the sockets
 * 
 * Initialize the socket file descriptor for receiving and sending sockets.
 * Assigns a fixed port to the receiving socket so clients know where to send
 * requests to.
 */
void init_sockets();

/**
 * @brief Sends a response message to a destination address and port.
 * 
 * @param destAddress Client IP address to send response to
 * @param destPort Client port to send response to
 * @param message Message to be sent
 */
void send_message(string destAddress, string destPort, string message);

/* 
Request processing 
*/

/**
 * @brief Decodes the request and assigns it to its handler
 * 
 * @param request Request received
 */
void process_request(string request);

/**
 * @brief Handler for fetch last modified time command
 * 
 * @param destAddress Client IP address to send response to
 * @param destPort Client port to send response to
 * @param jobjReceived CJSON representation of the request received
 */
void execute_fetch_last_modified_time_command(string destAddress,
                                              string destPort,
                                              cJSON *jobjReceived);

/**
 * @brief Handler for fetch last modified time command
 * 
 * @param destAddress Client IP address to send response to
 * @param destPort Client port to send response to
 * @param jobjReceived CJSON representation of the request received
 */
void execute_read_command(string destAddress, string destPort,
                          cJSON *jobjReceived);

/**
 * @brief Handler for write command
 * 
 * @param destAddress Client IP address to send response to
 * @param destPort Client port to send response to
 * @param jobjReceived CJSON representation of the request received
 */
void execute_write_command(string destAddress, string destPort,
                           cJSON *jobjReceived);

/**
 * @brief Handler for register command
 * 
 * @param destAddress Client IP address to send response to
 * @param destPort Client port to send response to
 * @param jobjReceived CJSON representation of the request received
 */
void execute_register_command(string destAddress, string destPort,
                              cJSON *jobjReceived);

/**
 * @brief Handler for clear file command
 * 
 * @param destAddress Client IP address to send response to
 * @param destPort Client port to send response to
 * @param jobjReceived CJSON representation of the request received
 */
void execute_clear_file_command(string destAddress, string destPort,
                                cJSON *jobjReceived);

/* 
Registered client monitoring 
*/

/**
 * @brief Monitors registered clients on a separate thread
 * 
 * @param ptr 
 * @return void* 
 */
void *monitor_registered_clients(void *ptr);

/**
 * @brief Sends a message to inform the client that the file being monitored has been updated.
 * 
 * @param sourceAddress Client IP address to send response to
 * @param destPort Client port to send response to
 */
void update_registered_client(string sourceAddress, string destPort);

// void expire_registered_client(string sourceAddress, string destPort);

/* 
RMI scheme request and response storage 
*/

/**
 * @brief Checks if the server has received this request previously.
 * 
 * @param destAddress Client IP address where request was sent from
 * @param destPort Client port where request was sent from
 * @param message Request message
 * @return true Request exists
 * @return false Request does not exist
 */
bool is_request_exists(string destAddress, string destPort, string message);

/**
 * @brief Stores the client's request
 * 
 * @param destAddress Client IP address where request was sent from
 * @param destPort Client port where request was sent from
 * @param message Request message
 */
void store_request(string destAddress, string destPort, string message);

/**
 * @brief Stores the server's response
 * 
 * @param destAddress Client IP address to send response to
 * @param destPort Client port to send response to
 * @param message Response message
 */
void store_response(string destAddress, string destPort, string message);

/**
 * @brief Retrieves the server's latest response to this client
 * 
 * @param destAddress Client IP address to send response to
 * @param destPort Client port to send response to
 * @return string Response message
 */
string retrieve_response(string destAddress, string destPort);

/* 
Getter methods for retrieving message fields 
*/

/**
 * @brief Get the request code object
 * 
 * @param jobjReceived CJSON representation of request message
 * @return int Request code
 */
int get_request_code(cJSON *jobjReceived);

/**
 * @brief Get the response id object
 * 
 * @param jobjReceived CJSON representation of request message
 * @return int Response id
 */
int get_response_id(cJSON *jobjReceived);

/**
 * @brief Get the offset object
 * 
 * @param jobjReceived CJSON representation of request message
 * @return int Byte offset
 */
int get_offset(cJSON *jobjReceived);

/**
 * @brief Get the nBytes object
 * 
 * @param jobjReceived CJSON representation of request message
 * @return int Number of bytes
 */
int get_nBytes(cJSON *jobjReceived);

/**
 * @brief Get the filepath object
 * 
 * @param jobjReceived CJSON representation of request message
 * @return string Filepath
 */
string get_filepath(cJSON *jobjReceived);

/**
 * @brief Get the monitor duration object
 * 
 * @param jobjReceived CJSON representation of request message
 * @return string Monitor duration
 */
string get_monitor_duration(cJSON *jobjReceived);

/**
 * @brief Get the dest port object
 * 
 * @param jobjReceived CJSON representation of request message
 * @return string Destination port
 */
string get_dest_port(cJSON *jobjReceived);

/**
 * @brief Get the content object
 * 
 * @param jobjReceived CJSON representation of request message
 * @return string Message content
 */
string get_content(cJSON *jobjReceived);

/* 
Methods to help with value conversion 
*/

/**
 * @brief Converts the result from FileHandler methods to a status the client understands.
 * 
 * @param result FileHandler method result
 * @return int Status
 */
int fh_read_status(int result);

/**
 * @brief Converts the result from FileHandler methods to a status the client understands.
 * 
 * @param result FileHandler method result
 * @return int Status
 */
int fh_write_status(int result);

/**
 * @brief Compares two times in time_t format
 * 
 * @param time1 
 * @param time2 
 * @return int 1 if time1 > time2, -1 otherwise
 */
int comparetime(time_t time1, time_t time2);

/**
 * @brief Converts pseudo_filepath to actual filepath
 * 
 * @param pseudo_filepath Filepath sent by client
 * @return string Actual filepath on server
 */
string translate_filepath(string pseudo_filepath);

/* 
Methods to retrieve system attributes 
*/

/**
 * @brief Get the last modified time object
 * 
 * @param path  Filepath of object to retrieve last modified time of
 * @return time_t Last modified time
 */
time_t get_last_modified_time(const char *path);

#endif