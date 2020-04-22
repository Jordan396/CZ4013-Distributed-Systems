/**
 * @file Debug.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Implementation code for Debug.cpp
 */

#include "Debug.h"

using namespace std;

namespace Debug
{
    void msg(string debug_message)
    {
        if (debugMode == 1) {
            cout << debug_message << endl;
        }   
        return;
    }
} 