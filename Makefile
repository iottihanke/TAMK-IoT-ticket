all:
	g++ main.cpp mqtt_arduino1.cpp SendWap.cpp -std=c++0x -pthread -lmosquitto -lmosquittopp -lcurl -ljsoncpp -o go
