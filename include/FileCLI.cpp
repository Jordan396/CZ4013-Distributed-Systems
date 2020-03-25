#include "FileCLI.h"

using namespace std;



void FileCLI::readFile()
{
	int fileID;

	listFile();
	cin >> fileID;
	
	// check if the fileID input is within bounds
	if (checkValidity(fileID)) {
		int offSet;
		int numBytes;


		cout << "Offset: ";
		cin >> offSet;
		cout << "Number of bytes: ";
		cin >> numBytes;

		// translate fileID to real remote file path 
		map<int, string>::iterator it;
		it = cacheReference.find(fileID);
		string remoteFilePath = it->second;
		
		// @ChongYan: You should be passing in local file path!
		cout << cv.readFile(remoteFilePath, offSet, numBytes) << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	}

	return;
}

void FileCLI::clearFile()
{
	int fileID;

	listFile();
	cin >> fileID;

	// check if the fileID input is within bounds
	if (checkValidity(fileID)) {

		// translate fileID to real remote file path 
		map<int, string>::iterator it;
		it = cacheReference.find(fileID);
		string remoteFilePath = it->second;
		
		if (cv.clearFile(remoteFilePath)) {
			cout << "File is removed" << endl;
		}
		else {
			cout << "Failed to remove file" << endl;
		}
	}
	return;
}

void FileCLI::writeFile()
{
	int fileID;

	listFile();
	cin >> fileID;

	// check if the fileID input is within bounds
	if (checkValidity(fileID)) {
		int offSet;
		string textAppend; 

		cout << "Offset: ";
		cin >> offSet;
		cout << "Input the text to be appended: ";
		cin >> textAppend;

		// translate fileID to real remote file path 
		map<int, string>::iterator it;
		it = cacheReference.find(fileID);
		string remoteFilePath = it->second;
		
		if (cv.writeFile(remoteFilePath, textAppend, offSet)) {
			cout << "Written into File Successfully" << endl;
		}
		else {
			cout << "Failed to write to file" <<endl;
		}
	}
	return;
}

void FileCLI::fetchFile()
{
	string filePath;
	cout << "Please input a file path: RFA://"; // line hit 
	cin >> filePath;
	string filePathModified = "RFA://" + filePath;
	cout << "filepath is: " + filePathModified << endl; // filepath = RFA://documents/test.txt
	if (cv.checkValidityFetch(filePathModified)) {
		cout << "File is fetched and cached" << endl; // line is never hit 
	}
	else {
		cout << "Failed to fetch and cache file" << endl;
	}

	return;
}



// table to show all the available files 
void FileCLI::listFile()
{
	int j;
	cacheKeys = cv.listCache();
	cacheSize = cacheKeys.size();
	for (j = 0; j < cacheKeys.size(); j++) {
		cacheReference.insert({ j +1 , cacheKeys[j] });
	}

	// get all the files available
	cout << "\u250f";  for (int i = 0; i < 40; i++) { cout << "\u2501"; }  cout << "\u2513" << endl;
	cout << "\u2503" << "Key in the FileID or -1 for manual input" << "\u2503" << endl;

	// no entries 
	if (j == 0) {
		cout << "\u2517";  for (int i = 0; i < 40; i++) { cout << "\u2501"; }  cout << "\u251b" << endl;
		return;
	}
	else {
		cout << "\u2523";  for (int i = 0; i < 4; i++) { cout << "\u2501"; } cout << "\u2533";  for (int i = 0; i < 35; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
	}

	for (int i = 0; i < cacheKeys.size(); i++) {
		cout << "\u2503" << i + 1; for (int i = 0; i < 4 - to_string(i + 1).length(); i++) { cout << " "; } cout << "\u2503" << cacheKeys[i];  for (int i = 0; i < 35 - cacheKeys[i].length(); i++) {
			cout << " ";} cout << "\u2503" << endl;

		if (i != cacheKeys.size() - 1) {
			cout << "\u2523";  for (int i = 0; i < 4; i++) { cout << "\u2501"; } cout << "\u254b";  for (int i = 0; i < 35; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
		}
		else {
			cout << "\u2517";  for (int i = 0; i < 4; i++) { cout << "\u2501"; }  cout << "\u253b";  for (int i = 0; i < 35; i++) { cout << "\u2501"; }  cout << "\u251b" << endl;
		}
	}

	return;
}

bool FileCLI::checkValidity(int fileID)
{
	if (fileID >= 1 && fileID <= cacheSize) {
		return true;
	}
	else {
		return false;
	}
}

FileCLI::~FileCLI()
{
	cv.saveHashMap();
}

FileCLI::FileCLI()
{
	// CacheService cv;
	cv.restoreHashMap();
}
