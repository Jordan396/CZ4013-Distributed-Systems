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
#define READ_CMD_CODE 0   
#define WRITE_CMD_CODE 1  
#define MONITOR_CMD_CODE 2 

/* Standard libraries */
#define _GNU_SOURCE
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
#include <sys/mman.h>   /* to create shared memory across child processes */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <sys/wait.h>   /* for waitpid() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */

/* External libraries */
#include "./../dependencies/RFAsockets.h"      // For UDPSocket and SocketException
#include "./../dependencies/cJSON.h"      // For message formatting

/**
 * @brief Accepts a cJSON object and sends its string representation over a socket.
 *
 * This function converts a cJSON object to its string representation.
 * It then sends this string to the other party on the network.
 * 
 * This payload adopts the following structure:
 * The first RCV_BUF_SIZE bytes indicates the size of the actual payload.
 * The remaining bytes contain the actual cJSON string representation payload.
 *
 * @param sock Client socket assigned to the connection.
 * @param jobjToSend cJSON object to be sent.
 * @return int 0 if error occurred, 1 otherwise.
 */
int send_message(int sock, cJSON *jobjToSend);

/**
 * @brief Receives a send_payload formatted response and saves it to objReceived.
 *
 * The socket listens for a send_payload formatted response.
 * It then saves the response to an objReceived string.
 * 
 * This reponse adopts the following structure:
 * The first RCV_BUF_SIZE bytes indicates the size of the actual payload.
 * The remaining bytes contain the actual cJSON string representation payload.
 *
 * @param sock Client socket assigned to the connection.
 * @param objReceived String to save the response recieved.
 * @return void
 */
void receive_message(int sock, char *objReceived);

/**
 * @brief Waits for secs amount of seconds.
 *
 * @param secs Number of seconds to wait for.
 * @return void
 */
void wait_for(unsigned int secs);