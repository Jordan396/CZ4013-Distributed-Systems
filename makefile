all: RFAcli RFAsrv

RFAsrv: ./server/RFAsrv.cpp ./dependencies/RFAsockets.cpp ./dependencies/cJSON.cpp
	g++ ./server/RFAsrv.cpp ./dependencies/RFAsockets.cpp ./dependencies/cJSON.cpp -o ./output/RFAsrv

RFAcli: ./client/RFAcli.cpp ./dependencies/RFAsockets.cpp ./dependencies/cJSON.cpp
	g++ ./client/RFAcli.cpp ./dependencies/RFAsockets.cpp ./dependencies/cJSON.cpp -o ./output/RFAcli
