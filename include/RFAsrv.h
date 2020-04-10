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

/* Standard libraries */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <chrono>
#include <cstring>
#include <ctime>
#include <ctype.h> /* for char validation */
#include <experimental/filesystem>
#include <fstream>
#include <functional>
#include <iostream> // For cout and cerr
#include <iterator>
#include <list>
#include <map>
#include <pthread.h>
#include <signal.h> /* for sigaction() */
#include <stack>
#include <stdio.h>  /* for printf() and fprintf() */
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <string>
#include <sys/mman.h>   /* to create shared memory across child processes */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <sys/stat.h>
#include <sys/wait.h> /* for waitpid() */
#include <thread>
#include <time.h>
#include <time.h>   /* for waitFor() */
#include <unistd.h> /* for close() */

#include "./FileHandler.h"
#include "./Global.h"
/* External libraries */
#include "./cJSON.h" // For message formatting
#include "./utils.h"

void init_sockets();
void *monitor_registered_clients(void *ptr);
int comparetime(time_t time1, time_t time2);
int send_message(string destAddress, string destPort, string message);
void process_request(string request);
void execute_fetch_last_modified_time_command(string destAddress,
                                              string destPort,
                                              cJSON *jobjReceived);
void execute_read_command(string destAddress, string destPort,
                          cJSON *jobjReceived);
void execute_write_command(string destAddress, string destPort,
                           cJSON *jobjReceived);
void execute_register_command(string destAddress, string destPort,
                              cJSON *jobjReceived);
void execute_clear_file_command(string destAddress, string destPort,
                                cJSON *jobjReceived);
void update_registered_client(string sourceAddress, string destPort);
bool is_request_exists(string destAddress, string destPort, string message);
void store_request(string destAddress, string destPort, string message);
void store_response(string destAddress, string destPort, string message);
string retrieve_response(string destAddress, string destPort);
int get_request_code(cJSON *jobjReceived);
int get_offset(cJSON *jobjReceived);
int get_nBytes(cJSON *jobjReceived);
string get_filepath(cJSON *jobjReceived);
string get_monitor_duration(cJSON *jobjReceived);
char *get_toWrite(cJSON *jobjReceived);
string translate_filepath(string pseudo_filepath);
time_t get_last_modified_time(const char *path);
string get_dest_port(cJSON *jobjReceived);
string get_content(cJSON *jobjReceived);
int get_response_id(cJSON *jobjReceived);
int fh_read_status(int result);
int fh_write_status(int result);

// /**
//  * @brief Accepts a cJSON object and sends its string representation over a
//  socket.
//  *
//  * This function converts a cJSON object to its string representation.
//  * It then sends this string to the other party on the network.
//  *
//  * This payload adopts the following structure:
//  * The first RCV_BUF_SIZE bytes indicates the size of the actual payload.
//  * The remaining bytes contain the actual cJSON string representation
//  payload.
//  *
//  * @param sock Client socket assigned to the connection.
//  * @param jobjToSend cJSON object to be sent.
//  * @return int 0 if error occurred, 1 otherwise.
//  */
// int send_message(int sock, cJSON *jobjToSend);

// /**
//  * @brief Receives a send_payload formatted response and saves it to
//  objReceived.
//  *
//  * The socket listens for a send_payload formatted response.
//  * It then saves the response to an objReceived string.
//  *
//  * This reponse adopts the following structure:
//  * The first RCV_BUF_SIZE bytes indicates the size of the actual payload.
//  * The remaining bytes contain the actual cJSON string representation
//  payload.
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