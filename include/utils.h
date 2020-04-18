/**
 * @file utils.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in utils.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>

namespace utils
{

/**
 * @brief determines if a packet has been artificially dropped
 * @param lossRate the desired loss rate
 * @return int 1 if the packet has not been dropped 0 otherwise 
 * */
int loss(int lossRate);
} // namespace utils

#endif