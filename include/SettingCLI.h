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

  /**
   * @brief Executed when the user wants to edit the setting
   * Method will display all the current settings and allow users to input new setting
   * @param void
   * @return void
   */
  void editSetting();
  ~SettingCLI();

private:

  /**
   * @brief Display all the current setting
   * @param void
   * @return void
   */
  void displayCurrentSetting();

  /**
   * @brief Check whether the string is numerical
   * @param s a string
   * @return bool true if is numerical, false if it contains other non-numerical characters
   */
  bool isNumber(std::string s);
};

#endif