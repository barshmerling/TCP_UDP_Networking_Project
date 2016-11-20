//============================================================================
// Name        : MessengerClientApp.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "TCPMessengerClient.h"

using namespace std;
using namespace npl;

void printInstructions() {
	cout << "Connect to the remote server type: c <ip>" << endl;
	cout << "Register type: register <user> <password>" << endl;
	cout << "Log in type: login <user> <password>" << endl;
	cout << "Print the users list from server by: lu" << endl;
	cout << "Print the connected users list by: lcu" << endl;
	cout << "Print available rooms by: lr" << endl;
	cout << "Print users in room by: lru <room name>" << endl;
	cout << "To open session with peer by: o <username>" << endl;
	cout << "To enter a chat room(or create if not exist) by: or <room name>" << endl;
	cout << "To send a message by: s <message>" << endl;
	cout << "Print user status by: l" << endl;
	cout << "To close the session/exit a room by: cs" << endl;
	cout << "To close a room(if you are the manager) by: cr" << endl;
	cout << "To disconnect from the server by: d" << endl;
	cout << "To exit by: x" << endl;
}

int main() {
	cout << "Welcome to TCP Messenger Client" << endl << flush;
	TCPMessengerClient* client = new TCPMessengerClient();
	printInstructions();

	while (true) {
		string msg;
		string command;
		cin >> command;
		if (command == "c") {
			string ip;
			cin >> ip;
			client->connect(ip);
		} else if(command == "lu"){
			client->listOfUsers();
		} else if(command == "lcu"){
			client->listConnectedUsers();
		}else if (command=="lr"){
			client->ListOfRooms();
		}else if (command=="lru"){
			string roomName;
			cin >> roomName;
			client->listUsersInRoom(roomName);
		}else if (command=="login"){
			string userName, password;
			cin >> userName;
			cin>> password;
			client->logIn(userName,password);
		}else if (command=="register"){
			string userName, password;
			cin >> userName;
			cin>> password;
			client->registr(userName, password);
		} else if (command == "o") {
			string userName;
			cin >> userName;
			client->openSession(userName);
		} else if (command == "or") {
			string roomName;
			cin >> roomName;
			client->openOrConnectRoom(roomName);
		} else if (command == "s") {
			getline(std::cin, msg);
			if (msg.size() > 0 && msg[0] == ' ')
				msg.erase(0, 1);
			client->send(msg);
		} else if (command == "cs") {
			client->exitRoomOrCloseSession();
		} else if (command == "cr") {
			string roomName;
			cin >> roomName;
			client->closeRoom(roomName);
		} else if (command == "l") {
			client->currentStatus();
		} else if (command == "d") {
			client->disconnect();
		} else if (command == "x") {
			client->exit();
			break;
		} else {
			cout << "wrong input" << endl;
		}
	}
	delete client;
	cout << "client was closed" << endl;
	return 0;
}
