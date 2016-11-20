//============================================================================
// Name        : MessengerServerApp.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string.h>
#include <string>
#include "TCPMessengerServer.h"

using namespace std;
using namespace npl;

int main() {
	TCPMessengerServer* server = new TCPMessengerServer();
	while(true){
		string str;
		string command;
		cin>>command;
		if(command == "x"){
			server->exit();
			break;
		}else if(command == "lu"){
			server->listAllUsers();
		}else if(command == "lcu"){
			server->listConnectedUsers();
		}else if(command == "ls"){
			server->listAllSessions();
		}else if(command == "lr"){
			server->listAllRooms();
		}else if(command == "lru"){
			string roomName;
			cin >> roomName;
			server->listUsersInRoom(roomName);
		}
		else{
			cout<<"Invalid Input"<<endl;
		}
	}
	delete server;
	cout<<"MessengerServer closed"<<endl;
	return 0;
}
