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
int loss(int lossRate)
{
  srand(time(NULL));
  int a = rand() % 100;
  Debug::msg("random number is: " + a);
  return (a > lossRate);
}
} // namespace utils