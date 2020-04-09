#include "File.h"

File::File(std::string lfp, std::chrono::system_clock::time_point ct)
	:localFilePath(lfp), createdTime(ct)
{
}

int File::computeTimeElapsed()
{
	time_t now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now - createdTime).count();
}

File::~File()
{
}



