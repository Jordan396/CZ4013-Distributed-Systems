/**
 * @file SettingCLI.h
 * @author Jordan396, leechongyan, seaerchin
 * @date 13 April 2020
 * @brief Documentation for functions in SettingCLI.h.
 *
 * Code is documented according to GNOME and Doxygen standards.
 * <https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en>
 * <http://www.doxygen.nl/manual/docblocks.html>
 */

#ifndef SETTING_CLI_H
#define SETTING_CLI_H

#include "Global.h"

#include <iostream>
#include <string>

class SettingCLI
{
public:
  void editSetting();
  ~SettingCLI();

private:
  void displayCurrentSetting();
  bool isNumber(std::string s);
};

#endif