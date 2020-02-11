all: RFAcli RFAsrv

RFAsrv: ./server/RFAsrv.cpp ./dependencies/PracticalSocket.cpp ./dependencies/cJSON.cpp
	g++ ./server/RFAsrv.cpp ./dependencies/PracticalSocket.cpp ./dependencies/cJSON.cpp -o ./output/RFAsrv

RFAcli: ./client/RFAcli.cpp ./dependencies/PracticalSocket.cpp ./dependencies/cJSON.cpp
	g++ ./client/RFAcli.cpp ./dependencies/PracticalSocket.cpp ./dependencies/cJSON.cpp -o ./output/RFAcli