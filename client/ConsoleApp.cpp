// ConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define __MAIN_C__
#include <iostream>
#include "FileCLI.h"
#include "SettingCLI.h"
#include "Global.h"
#include <locale>
#include <string>

using namespace std;
void displayClientMenu();

int freshnessInterval = 100;
int lossRate = 200;
int timeOut = 300;
int sel;
string serverIP = "";
string portNo = "";

int main(int argc, char* argv[])
{
    setlocale(LC_CTYPE, "");
    // read in command line arguments 
    // TODO to write serverno and portno input check
    if (argc == 7) {
        for (int i = 1; i < argc; i+=2) {
            string s1(argv[i]);
            if (s1=="-f") {
                freshnessInterval = atoi(argv[i+1]);
            }
            else if (s1=="-s") {
                string s2(argv[i+1]);
                serverIP = s2;
            }
            else if (s1=="-p") {
                string s2(argv[i + 1]);
                portNo = s2;
            }
        }
    }
    else {
        // if not enough arguments supplied, break program
        cout << argc;
        return -1;
    }
    // display client menu and listen
    while (true) {
        system("clear");
        displayClientMenu();
        cin >> sel;
        system("clear");
        switch (sel) {
        case 1: {
            FileCLI fileCLI;
            fileCLI.readFile();
            break;
        }
        case 2: {
            FileCLI fileCLI;
            fileCLI.clearFile();
            break;
        }
        case 3: {
            FileCLI fileCLI;
            fileCLI.appendFile();
            break;
        }
        case 4: {
            SettingCLI settingCLI;
            settingCLI.editSetting();
            break;
        }
        }
    }
}

// display client menu (main screen)
void displayClientMenu() {

    cout << "\u250f";  for (int i = 0; i < 40; i++) { cout << "\u2501"; }  cout << "\u2513" << endl;
    cout << "\u2503" << "CZ4031 Distributed System Main Menu     " << "\u2503" << endl;
    cout << "\u2523";  for (int i = 0; i < 4; i++) { cout << "\u2501"; } cout << "\u2533";  for (int i = 0; i < 35; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
    cout << "\u2503" << "1.  " << "\u2503" << "Read File                          " << "\u2503" << endl;
    cout << "\u2523";  for (int i = 0; i < 4; i++) { cout << "\u2501"; } cout << "\u254b";  for (int i = 0; i < 35; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
    cout << "\u2503" << "2.  " << "\u2503" << "Clear File                         " << "\u2503" << endl;
    cout << "\u2523";  for (int i = 0; i < 4; i++) { cout << "\u2501"; } cout << "\u254b";  for (int i = 0; i < 35; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
    cout << "\u2503" << "3.  " << "\u2503" << "Append to File                     " << "\u2503" << endl;
    cout << "\u2523";  for (int i = 0; i < 4; i++) { cout << "\u2501"; } cout << "\u254b";  for (int i = 0; i < 35; i++) { cout << "\u2501"; } cout << "\u252b" << endl;
    cout << "\u2503" << "4.  " << "\u2503" << "Settings                           " << "\u2503" << endl;
    cout << "\u2517";  for (int i = 0; i < 4; i++) { cout << "\u2501"; }  cout << "\u253b";  for (int i = 0; i < 35; i++) { cout << "\u2501"; }  cout << "\u251b" << endl;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
