#pragma once
#include "Global.h"
#include <string>
#include <iostream>
class SettingCLI
{
public:
	void editSetting();
	~SettingCLI();
private:
	void displayCurrentSetting();
	int selectionID;
};

