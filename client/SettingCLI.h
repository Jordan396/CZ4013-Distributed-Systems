#pragma once
#include <string>
class SettingCLI
{
public:
	void editSetting();
	~SettingCLI();
private:
	void displayCurrentSetting();
	int selectionID;
};

