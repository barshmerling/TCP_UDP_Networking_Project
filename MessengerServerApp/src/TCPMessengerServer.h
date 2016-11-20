/*
 * TCPMessengerServer.h
 *
 *  Created on: May 27, 2016
 *      Author: user
 */
/**
 * this class represent the server.
 * the class inherited from MThread and RegisterAndLogIn::Handler.
 * data members in the class:
 * TCPSocket* socket - an object of TCPSocket
 * Dispatcher* dispatcher - an object of dispatcher
 * MTCPListener* listener- an object of MTCPListener
 * methods in the class:
 * 	virtual void onLogin(RegisterAndLogIn* reglogin, User* user) - transpose the logged-in-user to the responsible of the dispatcher.
 * 	virtual void onRegister(RegisterAndLogIn* reglogin,const string& name,const string& pass) - add the registered-user to the map of registeredUsers in the dispatcher.
 */

#ifndef TCPMESSENGERSERVER_H_
#define TCPMESSENGERSERVER_H_

#include "TCPSocket.h"
#include "MThread.h"
#include "MTCPListener.h"
#include "Dispatcher.h"
#include "RegisterAndLogIn.h"

using namespace std;
namespace npl {

class TCPMessengerServer:public MThread, public RegisterAndLogIn::Handler {
private:
	TCPSocket* socket;
	Dispatcher* dispatcher;
	MTCPListener* listener;

public:
	TCPMessengerServer();
	void run();
	void listAllUsers();
	void listConnectedUsers();
	void listAllSessions();
	void listAllRooms();
	void listUsersInRoom(string roomName);
	virtual void onLogin(RegisterAndLogIn* reglogin, User* user);
	virtual void onRegister(RegisterAndLogIn* reglogin,const string& name,const string& pass);
	void exit();
	virtual ~TCPMessengerServer();
};

} /* namespace npl */

#endif /* TCPMESSENGERSERVER_H_ */
