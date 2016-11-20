/*
 * Dispatcher.h
 *
 *  Created on: May 27, 2016
 *      Author: user
 */
/**
 * this class represent the Dispatcher.
 * the Dispatcher listen to all the peers and handle the commands which incoming from the peers.
 * the class inherited from MThread, Session::SessionHandler, ChatRoom::ChatRoomHandler.
 * data members in the class:
 * vector<User*> users - a vector of Users
 * vector<Session*> sessions- a vector of Sessions
 * vector<ChatRoom*> chatRooms- a vector of ChatRooms
 * MTCPListener* listener- an object of MTCPListener
 * map<string,string> registeredUsers - mapping <userName, password> and holds information about the registered users
 * vector<User*> loggedInUsers - a vector of Users, holds information about the logged in users
 * methods in the class:
 * virtual void onClose(Session* session, User* userA, User* userB)- delete a session, close the threads and return the users were in a session to the manage of the dispatcher
 * virtual void onExit(Session* session, User* disconnectingUser, User* user) - when a client who an apart of a session entered exit
 * virtual void onDisconnect(ChatRoom* chatRoom, User* disconnectingUser) - when a client entered disconnect
 * virtual void onClose(ChatRoom* chatRoom, vector<User*> usersVector) - delete a chatRoom and close the threads
 * virtual void onExit(ChatRoom* chatRoom, User* disconnectingUser) - when a client which member in a room entered exit
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_
#define SIZE 32


#include "TCPSocket.h"
#include "MTCPListener.h"
#include "TCPMessengerProtocol.h"
#include "MThread.h"
#include "Session.h"
#include "User.h"
#include "ChatRoom.h"
#include "File.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

namespace npl {

class Dispatcher:public MThread, Session::SessionHandler, ChatRoom::ChatRoomHandler {

private:

	vector<User*> users;
	vector<Session*> sessions;
	vector<ChatRoom*> chatRooms;
	MTCPListener* listener;
	map<string,string> registeredUsers; // mapping <userName, password>
	vector<User*> loggedInUsers;
	bool running;
	void login(TCPSocket* peer, const string& data);

public:

	Dispatcher();
	void run();
	void listAllUsers();
	void listConnectedUsers();
	void listAllSessions();
	void listAllRooms();
	void listUsersInRoom(string roomName);

	void listOfUsers(TCPSocket* peer);
	void listConnectedUsers(TCPSocket* peer);
	void listOfRooms(TCPSocket* peer);
	void listUsersInRoom(TCPSocket* peer, const string& data);

	void addUser(User* user);
	void addToRegistered(const string& userName, const string& pass);
	void removeUser(User* user);
	void removeSession(Session* session);
	void removeChatRoom(ChatRoom* chatRoom);
	void openSession(User* user,const string& data);
	void openOrConnectRoom(User* user,const string& roomName);
	bool isLoggedIn(User* user);
	bool isRoomExist(const string& roomName);
	User* getUserByPeer(TCPSocket* peer);
	ChatRoom* getRoomByName(const string& roomName);
	void exit(User* user);

	virtual void onClose(Session* session, User* userA, User* userB);
	virtual void onExit(Session* session, User* disconnectingUser, User* user);
	virtual void onDisconnect(ChatRoom* chatRoom, User* disconnectingUser);
	virtual void onClose(ChatRoom* chatRoom, vector<User*> usersVector);
	virtual void onExit(ChatRoom* chatRoom, User* disconnectingUser);
	void close();
	virtual ~Dispatcher();
	void setRegisteredUsers();
};

} /* namespace npl */

#endif /* DISPATCHER_H_ */
