/**
 * @file SettingCLI.cpp
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Implementation code for SettingCLI.cpp
 */

#include "SettingCLI.h"

using namespace std;

void SettingCLI::editSetting()
{
  displayCurrentSetting();

  string selectionID;
  cin >> selectionID;

  if (!isNumber(selectionID))
  {
    return;
  }

  if (stoi(selectionID) < 1 && stoi(selectionID) > 5)
  {
    return;
  }

  switch (stoi(selectionID))
  {
  case 1:
  {
    cout << "Input a new Freshness Interval: ";
    cin >> freshnessInterval;
    system("clear");
    editSetting();
    break;
  }
  case 2:
  {
    cout << "Input a new Loss Rate: ";
    cin >> lossRate;
    system("clear");
    editSetting();
    break;
  }
  case 3:
  {
    cout << "Input a new Time Out: ";
    cin >> timeOut;
    system("clear");
    editSetting();
    break;
  }
  case 4:
  {
    cout << "Input a new server IP address: ";
    cin >> serverIP;
    system("clear");
    editSetting();
  }
  case 5:
  {
    cout << "Input a new server port number: ";
    cin >> serverPortNo;
    system("clear");
    editSetting();
  }
  }
  return;
}

void SettingCLI::displayCurrentSetting()
{
  cout << "\u250f";
  for (int i = 0; i < 77; i++)
  {
    cout << "\u2501";
  }
  cout << "\u2513" << endl;
  cout << "\u2503"
       << "Current Simulation Setting (Select ID to set or any character to ex"
          "it)       "
       << "\u2503" << endl;
  // sample for testing
  cout << "\u2523";
  for (int i = 0; i < 28; i++)
  {
    cout << "\u2501";
  }
  cout << "\u2533";
  for (int i = 0; i < 48; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "1. Freshness Interval (ms)  "
       << "\u2503" << freshnessInterval;
  for (int i = 0; i < 48 - to_string(freshnessInterval).length(); i++)
  {
    cout << " ";
  }
  cout << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 28; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 48; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "2. Loss Rate (%)            "
       << "\u2503" << lossRate;
  for (int i = 0; i < 48 - to_string(lossRate).length(); i++)
  {
    cout << " ";
  }
  cout << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 28; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 48; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "3. Time Out (ms)            "
       << "\u2503" << timeOut;
  for (int i = 0; i < 48 - to_string(timeOut).length(); i++)
  {
    cout << " ";
  }
  cout << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 28; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 48; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "4. Server IP                "
       << "\u2503" << serverIP;
  for (int i = 0; i < 48 - serverIP.length(); i++)
  {
    cout << " ";
  }
  cout << "\u2503" << endl;
  cout << "\u2523";
  for (int i = 0; i < 28; i++)
  {
    cout << "\u2501";
  }
  cout << "\u254b";
  for (int i = 0; i < 48; i++)
  {
    cout << "\u2501";
  }
  cout << "\u252b" << endl;
  cout << "\u2503"
       << "5. Server Port No.          "
       << "\u2503" << serverPortNo;
  for (int i = 0; i < 48 - serverPortNo.length(); i++)
  {
    cout << " ";
  }
  cout << "\u2503" << endl;
  cout << "\u2517";
  for (int i = 0; i < 28; i++)
  {
    cout << "\u2501";
  }
  cout << "\u253b";
  for (int i = 0; i < 48; i++)
  {
    cout << "\u2501";
  }
  cout << "\u251b" << endl;
}

SettingCLI::~SettingCLI() {}

/**
 * @brief Check whether the string is numerical
 * @param s a string
 * @return bool true if is numerical, false if it contains other non-numerical characters
 */
bool SettingCLI::isNumber(string s)
{
  for (int i = 0; i < s.length(); i++)
    if (isdigit(s[i]) == false)
      return false;

  return true;
}