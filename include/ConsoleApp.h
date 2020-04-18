/**
 * @file ConsoleApp.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in ConsoleApp.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef __MAIN_C__
#define __MAIN_C__

#include "CacheService.h"
#include "FileCLI.h"
#include "Global.h"
#include "SettingCLI.h"

#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <sys/mman.h>   /* to create shared memory across child processes */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <sys/wait.h>   /* for waitpid() */
#include <thread>
#include "Debug.h"
int freshnessInterval = 100;
int lossRate = 0;
int timeOut = 1000;
int bufferSize = 1024;
int udpDatagramSize = 4096;
int sel;
int debugMode = 1;
string serverIP = "";
string serverPortNo = "";
string clientPortNo = "";

/** 
 * @brief this is a driver function to display the various menu functions 
 * @param void
 * @return void 
 * */
void displayClientMenu();

/**
 * @brief Exits the program
 * 
 */
void exiting();

#endif