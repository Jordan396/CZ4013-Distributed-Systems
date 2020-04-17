#include "utils.h"

#include <iostream>

using namespace std;

namespace utils
{
int loss(int lossRate)
{
  srand(time(NULL));
  int a = rand() % 100;
  cout << "random number is: " << a << endl;
  return (a > lossRate);
}
} // namespace utils