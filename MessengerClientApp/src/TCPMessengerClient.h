/*
 * TCPMessengerClient.h
 *
 *  Created on: May 27, 2016
 *      Author: user
 */
/**
 * this class represent the client.
 * the class inherited from MThread.
 * each client is running  on thread ,connecting on TCP, and  between two clients the communication is on UDP.
 * data members in the class:
 * string clientName
 * string roomName - the current name of the room which the client belongs to
 * Status cStatus - the object of the Status
 * int myPort - the connected port
 * TCPSocket* myServer - an object of TCPSocket
 * UDPSocket* udpSocket - an object of UDPSocket
 * UDPMessengerClient* udpMessengerClient - an object of UDPMessengerClient
 * pair<string, string>* peerInSession - the peer which in session with the current client
 * map<string,string>* peersInRoom - mapping <userName, ip:port>
 */

#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <map>
#include <vector>
#include "MThread.h"
#include "TCPSocket.h"
#include "UDPMessengerClient.h"
#include "TCPMessengerProtocol.h"
#include "Status.h"

#ifndef TCPMESSENGERCLIENT_H_
#define TCPMESSENGERCLIENT_H_

using namespace std;

namespace npl {

class TCPMessengerClient :public MThread{
private:
	string clientName;
	string roomName;
	Status cStatus;
	int myPort;
	TCPSocket* myServer;
	UDPSocket* udpSocket;
	UDPMessengerClient* udpMessengerClient;
	pair<string, string>* peerInSession; //mapping <userName, ip:port>
	map<string,string>* peersInRoom; //mapping <userName, ip:port>

public:
	TCPMessengerClient();
	void run();
	void connect(const string& ip);
	void registr(const string& userName, const string& password);
	void logIn(const string& userName, const string& password);
	void currentStatus();

	void listOfUsers();
	void listConnectedUsers();
	void ListOfRooms();
	void listUsersInRoom(string roomName);

	void openSession(const string& clientName);
	void openOrConnectRoom(const string& roomName);

	void send(const string& msg);
	void exitRoomOrCloseSession();
	void closeRoom(string roomName);
	void disconnect();
	void exit();
	virtual ~TCPMessengerClient();

private:
	void close();
};

} /* namespace npl */

#endif /* TCPMESSENGERCLIENT_H_ */
