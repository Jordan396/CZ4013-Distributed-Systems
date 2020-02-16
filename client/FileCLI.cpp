#include "FileCLI.h"
#include <iostream>
using namespace std;

void FileCLI::readFile()
{
	listFile();
	getFileID();
	cout << "Offset: ";
	cin >> offSet;
	cout << "Number of bytes: ";
	cin >> numBytes;
	// TODO: execute operation reading file 

}

void FileCLI::clearFile()
{
	listFile();
	getFileID();
	// TODO: execute clearing file content

}

void FileCLI::appendFile()
{
	listFile();
	getFileID();
	cout << "Offset: ";
	cin >> offSet;
	cout << "Input the text to be appended: ";
	cin >> textAppend;
	// TODO: execute appending file
}



// table to show all the available files 
void FileCLI::listFile()
{
	// get all the files available
	cout << "\u250f";  for (int i = 0; i < 40; i++) { cout << "\u2501"; }  cout << "\u2513" << endl;
	cout << "\u2503" << "Key in the FileID or -1 for manual input" << "\u2503" << endl;
	// sample for testing
	cout << "\u2523";  for (int i = 0; i < 4; i++) { cout << "\u2501"; } cout << "\u2533";  for (int i = 0; i < 35; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
	cout << "\u2503" << "1.  " << "\u2503" << "/root/user123/sample.txt           " << "\u2503" << endl;
	cout << "\u2523";  for (int i = 0; i < 4; i++) { cout << "\u2501"; } cout << "\u254b";  for (int i = 0; i < 35; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
	cout << "\u2503" << "2.  " << "\u2503" << "/root/user123/sample.txt           " << "\u2503" << endl;
	cout << "\u2517";  for (int i = 0; i < 4; i++) { cout << "\u2501"; }  cout << "\u253b";  for (int i = 0; i < 35; i++) { cout << "\u2501"; }  cout << "\u251b" << endl;
}

void FileCLI::getFileID()
{
	cin >> fileID;
	//	manual input if user key in -1
	if (fileID == -1) {
		cin >> filePathName;
	}
}
