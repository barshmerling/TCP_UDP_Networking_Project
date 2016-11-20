/*
 * TCPMessengerServer.cpp
 *
 *  Created on: May 27, 2016
 *      Author: user
 */

#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"

#define USERS_PATH "users.txt"

using namespace std;
namespace npl{

TCPMessengerServer::TCPMessengerServer() {
	dispatcher = new Dispatcher();
	socket = new TCPSocket(MSNGR_PORT);
	start();
}

void TCPMessengerServer::run(){
	RegisterAndLogIn* registerAndLogIn = new RegisterAndLogIn(this, "/home/user/Desktop/workspace2/users.txt");
	while (true) {
		TCPSocket* peer = socket->listenAndAccept();
		if (peer == NULL) {
			break;
		}
		registerAndLogIn->addPeerToListen(peer);
	}
}

void TCPMessengerServer::listAllUsers(){
	dispatcher->listAllUsers();
}

void TCPMessengerServer::listConnectedUsers(){
	dispatcher->listConnectedUsers();
}

void TCPMessengerServer::listAllSessions(){
	dispatcher->listAllSessions();
}

void TCPMessengerServer::listAllRooms(){
	dispatcher->listAllRooms();
}

void TCPMessengerServer::listUsersInRoom(string roomName){
	dispatcher->listUsersInRoom(roomName);
}

void TCPMessengerServer::exit(){
	socket->close();
	waitForThread();
	delete socket;
	dispatcher->close();
	delete dispatcher;
}

void TCPMessengerServer::onLogin(RegisterAndLogIn* reglogin, User* user){
	dispatcher->addUser(user);
}

void TCPMessengerServer::onRegister(RegisterAndLogIn* reglogin,const string& name,const string& pass){
	dispatcher->addToRegistered(name,pass);
}

TCPMessengerServer::~TCPMessengerServer() {
}

}
