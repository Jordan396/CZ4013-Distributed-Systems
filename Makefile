all: ConsoleApp RFAsrv

ConsoleApp: ./include/ConsoleApp.cpp ./include/FileCLI.cpp ./include/SettingCLI.cpp ./include/CacheManager/CacheService.cpp ./include/CacheManager/File.cpp ./include/RFAcli.cpp ./include/FileHandler.cpp ./include/cJSON.cpp 
	g++		./include/ConsoleApp.cpp ./include/FileCLI.cpp ./include/SettingCLI.cpp ./include/CacheManager/CacheService.cpp ./include/CacheManager/File.cpp ./include/RFAcli.cpp ./include/FileHandler.cpp ./include/cJSON.cpp -o ./bin/ConsoleApp -lstdc++fs

RFAsrv: ./include/RFAsrv.cpp ./include/cJSON.cpp ./include/FileHandler.cpp
	g++ ./include/RFAsrv.cpp ./include/cJSON.cpp ./include/FileHandler.cpp -pthread -o ./bin/RFAsrv -lstdc++fs
