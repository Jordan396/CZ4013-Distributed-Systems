#include "SettingCLI.h"
#include "Global.h"
#include <string>
#include <iostream>
using namespace std;

void SettingCLI::editSetting()
{
	displayCurrentSetting();
	cin >> selectionID;
	switch (selectionID) {
	case 1: {
		cout << "Input a new Freshness Interval: ";
		cin >> freshnessInterval;
		system("clear");
		editSetting();
		break;
	}
	case 2: {
		cout << "Input a new Loss Rate: ";
		cin >> lossRate;
		system("clear");
		editSetting();
		break;
	}
	case 3: {
		cout << "Input a new Time Out: ";
		cin >> timeOut;
		system("clear");
		editSetting();
		break;
	}
	case -1: {
		return;
	}
	}
	return;
}

void SettingCLI::displayCurrentSetting()
{
	cout << "\u250f";  for (int i = 0; i < 72; i++) { cout << "\u2501"; }  cout << "\u2513" << endl;
	cout << "\u2503" << "Current Simulation Setting (Select ID to set or -1 to exit to main menu)" << "\u2503" << endl;
	// sample for testing
	cout << "\u2523";  for (int i = 0; i < 23; i++) { cout << "\u2501"; } cout << "\u2533";  for (int i = 0; i < 48; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
	cout << "\u2503" << "1. Freshness Interval  " << "\u2503" << freshnessInterval; for (int i = 0; i < 48 - to_string(freshnessInterval).length(); i++) {
		cout << " ";}  cout << "\u2503" << endl;
	cout << "\u2523";  for (int i = 0; i < 23; i++) { cout << "\u2501"; } cout << "\u254b";  for (int i = 0; i < 48; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
	cout << "\u2503" << "2. Loss Rate           " << "\u2503" << lossRate; for (int i = 0; i < 48 - to_string(lossRate).length(); i++) {
		cout << " ";}  cout << "\u2503" << endl;
	cout << "\u2523";  for (int i = 0; i < 23; i++) { cout << "\u2501"; } cout << "\u254b";  for (int i = 0; i < 48; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
	cout << "\u2503" << "3. Time Out            " << "\u2503" << timeOut; for (int i = 0; i < 48 - to_string(timeOut).length(); i++) {
		cout << " ";}  cout << "\u2503" << endl;
	cout << "\u2523";  for (int i = 0; i < 23; i++) { cout << "\u2501"; } cout << "\u254b";  for (int i = 0; i < 48; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
	cout << "\u2503" << "4. Server IP           " << "\u2503" << serverIP; for (int i = 0; i < 48 - serverIP.length(); i++) {
		cout << " ";
	}  cout << "\u2503" << endl;
	cout << "\u2523";  for (int i = 0; i < 23; i++) { cout << "\u2501"; } cout << "\u254b";  for (int i = 0; i < 48; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
	cout << "\u2503" << "5. Server Port No.     " << "\u2503" << portNo; for (int i = 0; i < 48 - portNo.length(); i++) {
		cout << " ";
	}  cout << "\u2503" << endl;
	cout << "\u2517";  for (int i = 0; i < 23; i++) { cout << "\u2501"; }  cout << "\u253b";  for (int i = 0; i < 48; i++) { cout << "\u2501"; }  cout << "\u251b" << endl;
}
