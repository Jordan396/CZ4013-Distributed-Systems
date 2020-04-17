/**
 * @file ConsoleApp.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Implementation code for ConsoleApp.cpp
 */

#include "ConsoleApp.h"

using namespace std;

// ConsoleApp.cpp : This file contains the 'main' function. Program execution
// begins and ends there.

int main(int argc, char *argv[])
{

  std::atexit(exiting);
  setlocale(LC_CTYPE, "");
  // read in command line arguments
  // -f for freshness interval, -s for server IP, -sp for server port number, -cp for client port number, -l for loss rate
  if (argc == 11)
  {
    for (int i = 1; i < argc; i += 2)
    {
      string s1(argv[i]);
      if (s1 == "-f")
      {
        freshnessInterval = atoi(argv[i + 1]);
      }
      else if (s1 == "-s")
      {
        string s2(argv[i + 1]);
        serverIP = s2;
      }
      else if (s1 == "-sp")
      {
        string s2(argv[i + 1]);
        serverPortNo = s2;
      }
      else if (s1 == "-cp")
      {
        string s2(argv[i + 1]);
        clientPortNo = s2;
      }
      else if (s1 == "-l")
      {
        lossRate = atoi(argv[i + 1]);
      }
    }
  }
  else
  {
    // if not enough arguments supplied, break program
    cout << argc;
    return -1;
  }

  FileCLI fileCLI;
  SettingCLI settingCLI;
  // display client menu and listen
  while (true)
  {
    system("clear");
    displayClientMenu();
    cin >> sel;
    system("clear");
    switch (sel)
    {
    case 1:
    {
      fileCLI.fetchFile();
      break;
    }
    case 2:
    {
      fileCLI.readFile();
      break;
    }
    case 3:
    {
      fileCLI.writeFile();
      break;
    }
    case 4:
    {
      fileCLI.clearContent();
      break;
    }
    case 5:
    {
      fileCLI.clearFile();
      break;
    }
    case 6:
    {
      fileCLI.clearCache();
      break;
    }
    case 7:
    {
      fileCLI.monitorFile();
      break;
    }
    case 8:
    {
      settingCLI.editSetting();
      break;
    }
    case 9:
    {
      goto endloop;
    }
    }
  }
endloop:;
}

// display client menu (main screen)
void displayClientMenu()
{

  cout << "\u250f";
  for (int i = 0; i < 40; i++)
  {
    cout << "\u2501";
  }
  cout << "\u2513" << endl;
  cout << "\u2503"
       << "CZ4031 Distributed System Main Menu     "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u2533";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "1.  "
       << "\u2503"
       << "Fetch File                         "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "2.  "
       << "\u2503"
       << "Read File                          "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "3.  "
       << "\u2503"
       << "Write File                         "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "4.  "
       << "\u2503"
       << "Clear Content in File              "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "5.  "
       << "\u2503"
       << "Remove File in Cache               "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "6.  "
       << "\u2503"
       << "Clear Cache                        "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "7.  "
       << "\u2503"
       << "Monitor File                       "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "8.  "
       << "\u2503"
       << "Settings                           "
       << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "9.  "
       << "\u2503"
       << "Exit Program                       "
       << "\u2503" << endl;
  cout << "\u2517";
  for (int i = 0; i < 4; i++)
  {
    cout << "\u2501";
  }
  cout << "\u253b";
  for (int i = 0; i < 35; i++)
  {
    cout << "\u2501";
  }
  cout << "\u251b" << endl;
}

void exiting()
{
  // cv.saveHashMap();
}
