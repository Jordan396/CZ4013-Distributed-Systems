#include "File.h"

File::File(std::string lfp, time_t ct) : localFilePath(lfp), createdTime(ct) {}

double File::computeTimeElapsed() {
  time_t now;
  time(&now);
  return difftime(now, createdTime);
}

File::~File() {}
