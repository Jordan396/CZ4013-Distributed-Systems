#include "File.h"
using namespace std;

File::File(std::string lfp, std::chrono::system_clock::time_point ct)
	:localFilePath(lfp), createdTime(ct)
{
}

int File::computeTimeElapsed()
{
	chrono::system_clock::time_point now = chrono::system_clock::now();
	return chrono::duration_cast<chrono::milliseconds>(now - createdTime).count();
};


