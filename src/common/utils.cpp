/**
 * @file utils.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Implementation code for utils.cpp
 */

#include "utils.h"

#include <iostream>

using namespace std;

namespace utils
{
/**
 * @brief determines if a packet has been artificially dropped
 * @param lossRate the desired loss rate
 * @return int 1 if the packet has not been dropped 0 otherwise 
 * */
int loss(int lossRate)
{
  srand(time(NULL));
  int a = rand() % 100;
  cout << "random number is: " << a << endl;
  return (a > lossRate);
}
} // namespace utils