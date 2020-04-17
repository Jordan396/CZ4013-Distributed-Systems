/**
 * @file Global.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in Global.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#ifndef __externs_h__
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <string>
#include <sys/socket.h>
#endif

#ifndef __MAIN_C__
extern int freshnessInterval;
extern int lossRate;
extern int timeOut;
extern int bufferSize;
extern int udpDatagramSize;
extern std::string serverIP;
extern std::string serverPortNo;
extern std::string clientPortNo;
#endif

#endif
