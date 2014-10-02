//
// main.cc
//
// Author: Jordan Brown
// Date: Sep 1, 2014
//
// Entry point for client/server application

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include "globals.h"
#include "server/request_handler.h"
#include "server/server_connection.h"
#include "client/client_interface.h"
#include "service_request.h"

using namespace brown;

std::string* loadPortNums();
void loadNeighbors();
void validateInput(char* lowPort, char* highPort);
void* launchClientInterface(void* args);

int main(int argc, char** argv) {
	std::string* portnums = loadPortNums();
	loadNeighbors();

	char* lowPort = (char*)portnums[0].c_str();
	char* highPort = (char*)portnums[1].c_str();
	validateInput(lowPort, highPort);
	initGlobals();

	server_connection serverConnection(atoi(lowPort), atoi(highPort));
	serverConnection.openConnection();

	pthread_t clientInterfaceThread;
	pthread_create(&clientInterfaceThread, NULL, launchClientInterface, (void*)serverConnection.getPort());

	request_handler handler(serverConnection.getSocketDesc(), serverConnection.getPort());
	handler.serviceRequests();
}

std::string* loadPortNums() {
	fileManager.openPortNumsFile();
	std::string* portnums = fileManager.readPortNumsFile(new std::string[2]);
	fileManager.closePortNumsFile();
	return portnums;
}

void loadNeighbors() {
	fileManager.openNeighborsFile();
	fileManager.readNeighborsFile();
	fileManager.closeNeighborsFile();
}

void validateInput(char* lowPort, char* highPort) {
	int lowPortInt = atoi(lowPort);
	int highPortInt = atoi(highPort);
	if(strlen(lowPort) == 0 || strlen(highPort) == 0) {
		std::cout << "portnums: The file must provide lower and upper port boundaries"
				<< std::endl;
		exit(0);
	}
	if(lowPortInt > highPortInt) {
		std::cout << "portnums: The lower port boundary must be less than the higher port boundary"
				<< std::endl;
		exit(0);
	}
	if(lowPortInt < PORT_MINIMUM || highPortInt < PORT_MINIMUM) {
		std::cout << "portnums: Port number boundaries must be greater than or equal to "
				<< PORT_MINIMUM << std::endl;
		exit(0);
	} else if (lowPortInt > PORT_MAXIMUM || highPortInt > PORT_MAXIMUM) {
		std::cout << "portnums: Port number boundaries must be less than or equal to "
				<< PORT_MAXIMUM << std::endl;
		exit(0);
	}
}

void* launchClientInterface(void* args) {
	char* port = (char*)args;
	client_interface interface(port);
	interface.initialize();
	pthread_exit(0);
	return NULL;
}
