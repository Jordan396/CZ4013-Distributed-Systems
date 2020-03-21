# all: ConsoleApp RFAsrv

# RFAsrv: ./include/RFAsrv.cpp ./include/RFAsockets.cpp ./include/cJSON.cpp
# 	g++ ./include/RFAsrv.cpp ./include/RFAsockets.cpp ./include/cJSON.cpp -o ./bin/RFAsrv

# ConsoleApp: ./include/ConsoleApp.cpp ./include/FileCLI.cpp ./include/SettingCLI.cpp ./include/CacheManager/CacheService.cpp ./include/CacheManager/File.cpp
# 	g++		./include/ConsoleApp.cpp ./include/FileCLI.cpp ./include/SettingCLI.cpp ./include/CacheManager/CacheService.cpp ./include/CacheManager/File.cpp -o ./bin/ConsoleApp -lstdc++fs

all: ConsoleApp
ConsoleApp: ./include/ConsoleApp.cpp ./include/FileCLI.cpp ./include/SettingCLI.cpp ./include/CacheManager/CacheService.cpp ./include/CacheManager/File.cpp ./include/RFAcli.cpp ./include/FileHandler.cpp ./include/cJSON.cpp ./include/RFAsockets.cpp
	g++		./include/ConsoleApp.cpp ./include/FileCLI.cpp ./include/SettingCLI.cpp ./include/CacheManager/CacheService.cpp ./include/CacheManager/File.cpp ./include/RFAcli.cpp ./include/FileHandler.cpp ./include/cJSON.cpp ./include/RFAsockets.cpp -o ./bin/ConsoleApp -lstdc++fs
