#pragma once
#include "Global.h"
#include <iostream>
#include <string>
class SettingCLI {
public:
  void editSetting();
  ~SettingCLI();

private:
  void displayCurrentSetting();
};
