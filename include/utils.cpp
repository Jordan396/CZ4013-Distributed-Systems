// taken from stackoverflow - decorator pattern in cpp???
// call it as decorate(int, func)(params)
#include "./utils.h"

namespace utils {
    bool loss(int lossRate) { 
        return ((rand()%100) > lossRate);
    }
}