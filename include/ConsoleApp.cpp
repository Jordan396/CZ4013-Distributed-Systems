// ConsoleApp.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
#define __MAIN_C__
#include "CacheManager/CacheService.h"
#include "FileCLI.h"
#include "Global.h"
#include "SettingCLI.h"
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <sys/mman.h>   /* to create shared memory across child processes */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <sys/wait.h>   /* for waitpid() */
#include <thread>

using namespace std;
void displayClientMenu();
void exiting();

int freshnessInterval = 100;
int lossRate = 0;
int timeOut = 1000;
int bufferSize = 1024;
int udpDatagramSize = 4096;
int sel;
string serverIP = "";
string serverPortNo = "";
string clientPortNo = "";
// CacheService cv;

int main(int argc, char *argv[]) {

  std::atexit(exiting);
  setlocale(LC_CTYPE, "");
  // read in command line arguments
  // TODO to write serverno and portno input check
  // -f for freshness interval, -s for server IP, -sp for server port number
  if (argc == 11) {
    for (int i = 1; i < argc; i += 2) {
      string s1(argv[i]);
      if (s1 == "-f") {
        freshnessInterval = atoi(argv[i + 1]);
      } else if (s1 == "-s") {
        string s2(argv[i + 1]);
        serverIP = s2;
      } else if (s1 == "-sp") {
        string s2(argv[i + 1]);
        serverPortNo = s2;
      } else if (s1 == "-cp") {
        string s2(argv[i + 1]);
        clientPortNo = s2;
      } else if (s1 == "-l") {
        lossRate = atoi(argv[i + 1]);
      }
    }
  } else {
    // if not enough arguments supplied, break program
    cout << argc;
    return -1;
  }

  FileCLI fileCLI;
  SettingCLI settingCLI;
  // display client menu and listen
  while (true) {
    system("clear");
    displayClientMenu();
    cin >> sel;
    system("clear");
    switch (sel) {
    case 1: {
      fileCLI.fetchFile();
      break;
    }
    case 2: {
      fileCLI.readFile();
      break;
    }
    case 3: {
      fileCLI.writeFile();
      break;
    }
    case 4: {
      fileCLI.clearContent();
      break;
    }
    case 5: {
      fileCLI.clearFile();
      break;
    }
    case 6: {
      fileCLI.clearCache();
      break;
    }
    case 7: {
      fileCLI.monitorFile();
      break;
    }
    case 8: {
      settingCLI.editSetting();
      break;
    }
    case 9: {
      goto endloop;
    }
    }
  }
endloop:;
}

// display client menu (main screen)
void displayClientMenu() {

  cout << "\u250f";
  for (int i = 0; i < 40; i++) {
    cout << "\u2501";
  }
  cout << "\u2513" << endl;
  cout << "\u2503"
       << "CZ4031 Distributed System Main Menu     "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++) {
    cout << "\u2501";
  }
  cout << "\u2533";
  for (int i = 0; i < 35; i++) {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "1.  "
       << "\u2503"
       << "Fetch File                         "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++) {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++) {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "2.  "
       << "\u2503"
       << "Read File                          "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++) {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++) {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "3.  "
       << "\u2503"
       << "Write File                         "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++) {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++) {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "4.  "
       << "\u2503"
       << "Clear Content in File              "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++) {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++) {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "5.  "
       << "\u2503"
       << "Remove File in Cache               "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++) {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++) {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "6.  "
       << "\u2503"
       << "Clear Cache                        "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++) {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++) {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "7.  "
       << "\u2503"
       << "Monitor File                      "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++) {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++) {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
      << "8.  "
      << "\u2503"
      << "Settings                           "
      << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++) {
      cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++) {
      cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "9.  "
       << "\u2503"
       << "Exit Program                       "
       << "\u2503" << endl;
  cout << "\u2517";
  for (int i = 0; i < 4; i++) {
    cout << "\u2501";
  }
  cout << "\u253b";
  for (int i = 0; i < 35; i++) {
    cout << "\u2501";
  }
  cout << "\u251b" << endl;
}

void exiting() {
  // cv.saveHashMap();
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add
//   Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project
//   and select the .sln file
