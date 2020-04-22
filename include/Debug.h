/**
 * @file Debug.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in Debug.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <cstdlib>
#include <iostream>
#include "Global.h"

namespace Debug
{

    /**
     * @brief print out the debugging message only when is in debug mode
     * @param debug_message the message to be print out
     * @return void
     * */
    void msg(std::string debug_message);
} 

#endif