#pragma once
#ifndef __externs_h__
#define __global_h__
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

#ifndef __MAIN_C__
extern int freshnessInterval;
extern int lossRate;
extern int timeOut;
extern int bufferSize;
extern std::string serverIP;
extern std::string serverPortNo;
extern std::string clientPortNo;
#endif

#endif
