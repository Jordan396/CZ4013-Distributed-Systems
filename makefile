all: ConsoleApp RFAsrv

RFAsrv: ./server/RFAsrv.cpp ./dependencies/RFAsockets.cpp ./dependencies/cJSON.cpp
	g++ ./server/RFAsrv.cpp ./dependencies/RFAsockets.cpp ./dependencies/cJSON.cpp -o ./output/RFAsrv

ConsoleApp: ./client/ConsoleApp.cpp ./client/FileCLI.cpp ./client/SettingCLI.cpp ./client/CacheManager/CacheService.cpp ./client/CacheManager/File.cpp
	g++		./client/ConsoleApp.cpp ./client/FileCLI.cpp ./client/SettingCLI.cpp ./client/CacheManager/CacheService.cpp ./client/CacheManager/File.cpp -o ./output/ConsoleApp -lstdc++fs

