#pragma once
#include "time.h"
#include <chrono>
#include <string>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/serialization/map.hpp>
class File {
public:
  File(std::string lfp, time_t ct);
  std::string localFilePath;
  time_t createdTime;
  double computeTimeElapsed();
  ~File();
};
