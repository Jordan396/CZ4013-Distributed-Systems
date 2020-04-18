/**
 * @file RFAcli.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in RFAcli.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef RFA_CLI_H
#define RFA_CLI_H

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

/**
 * @brief Network interface on the client side
 * 
 */
class RFAcli
{
public:
  /**
   * @brief Constructor for the client; it initializes the socket file descriptor for sending and receiving sockets.
   * 
   * This constructor initializes the socket file descriptor for the sending and receiving sockets. It also performs
   * binding on the receiving socket port, so that the machine knows which process to forward UDP datagrams arriving 
   * at port clientPortNo to.
   * 
   * @param void
   * @return void
   * */
  RFAcli();

  /**
   * @brief Destroy the RFAcli::RFAcli object
   * 
   */
  ~RFAcli();

  /****************************************************************************
   *                                                                          *
   *         Methods related to transferring messages over network            *
   *                                                                          *
   ****************************************************************************/

  /** 
   * @brief this method sends a message from the client across to the server 
   * @param message the string indicating the message the client wishes to send
   * @return int an integer indicating the success code 
   * */
  int send_message(string message);

  /** 
   * @brief this method receives a message across from the server
   * @param monitorFlag a boolean that indicates whether there should be monitoring on the client 
   * @return string the message the server sent 
   * */
  string receive_message(bool monitorFlag);

  /**
   * @brief Performs a non blocking send and receive loop
   * 
   * This method sends a message to the server and transitions into a blocking
   * receive state for a specified amount of time. If no messages are received
   * upon the expiration of this timeout, or if the message received does not 
   * corresponding to the request identifier of the message the client sent, this 
   * method will resend the message. This process repeats until the response
   * with a correct response id is received.
   * 
   * @param request Message to be sent
   * @return string Response received
   */
  string non_blocking_send_receive(string request);

  /**
   * @brief Sets the receiving socket to blocking receive mode for monitor_duration
   * 
   * @param monitor_duration Time to be in the blocking receive state
   * @return string Response received
   */
  string blocking_receive(int monitor_duration);

  /** 
 * @brief Resets the fields in the sockaddr destination address 
 * */
  void reset_destAddr();

  /****************************************************************************
   *                                                                          *
   *                    Methods to perform client requests                    *
   *                                                                          *
   ****************************************************************************/

  /** 
   * @brief this method downloads a file from the given remote filepath to the local filepath
   * @param remote_filepath the address of the specified file on the server
   * @param local_filepath the local address to save the file to 
   * @return integer the response code 
   * */
  int download_file(string remote_filepath, string local_filepath);

  /**
   * get last modified time of file on server
   * @param remote_filepath filepath that we want to query
   * @param last_modified_time local last modified time of requested file
   * @return time_t last modified time
   **/
  time_t fetch_last_modified_time(string remote_filepath);

  /** 
   * @brief this method writes a string to the remote file at the specified offset
   * @param toWrite this is the string to write to the remote file
   * @param nOffset the index that the string will be written at
   * @param remote_filepath the remote file to be written to 
   * @return void 
   * */
  void write_file(string remote_filepath, string toWrite, int nOffset);

  /** 
   * @brief this registers a client on the server and notifies the server that they would wish to be notified for any changes to a specified file
   * @param remote_filepath the file which the client wishes to monitor
   * @param local_filepath the local file to write any changes to
   * @param monitor_duration the duration that the client wishes to monitor the specified remote file
   * @return int the response code of the register 
   * */
  int register_client(string remote_filepath, string local_filepath,
                      string monitor_duration);

  /** 
   * @brief this method clears a remote file of its content - ie, it will become an empty file
   * @param remote_filepath the remote file which the client wishes to clear
   * @return void
   * */
  void clear_file(string remote_filepath);

  /****************************************************************************
   *                                                                          *
   *                    Getter methods to parse response                      *
   *                                                                          *
   ****************************************************************************/

  /** 
   * @brief this method gets the response code of the server from a specified cJSON struct
   * @param jobjReceived the pointer to the specific cJSON struct 
   * @return int the integer response code, obtained from the cJSON struct
   * */
  int get_response_code(cJSON *jobjReceived);

  /**
   * @brief this returns the number of bytes written from a specific cJSON struct
   * @param jobjReceived the pointer to the cJSON struct
   * @return int the number of bytes written 
   * */
  int get_nBytes(cJSON *jobjReceived);

  /** 
   * @brief this method returns the last modified time of a file from the cJSON struct
   * @param jobjReceived the pointer to the specific cJSON struct
   * @return time_t the last modified time of the file wrapped in a time_t struct 
   * */
  time_t get_last_modified_time(cJSON *jobjReceived);

  /** 
   * @brief this method gets the content from a specified cJSON struct 
   * @param jobjReceived the pointer to the specified cJSON struct
   * @return string content of the cJSON struct 
   * */
  string get_content(cJSON *jobjReceived);

  /**
   * @brief this returns the response id from a specific cJSON struct
   * @param jobjReceived the pointer to the cJSON struct
   * @return int response id
   * */
  int get_response_id(cJSON *jobjReceived);

private:
  FileHandler fh;
  int inboundSockFD, outboundSockFD;
  int monitorDuration;
  time_t registerTime;
  sockaddr_in destAddr = {0};   // Address and port of server
  sockaddr_in sourceAddr = {0}; // Address and port of client

  /**
   * @brief this initialises the socket on the client to connect to the server
   * @param monitorFlag a boolean that indicates whether there should be monitoring on the client 
   * @return void 
   * */
  void init_socket(bool monitorFlag);

  /** 
   * @brief this method displays the progress for the monitor duration
   * @param monitor_duration the specified duration to monitor for 
   * @return void
   * */
  void display_progress(int monitor_duration);
};

#endif