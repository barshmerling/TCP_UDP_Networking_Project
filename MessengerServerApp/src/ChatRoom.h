/*
 * ChatRoom.h
 *
 *  Created on: May 27, 2016
 *      Author: user
 */
/**
 * this class represent a chatRoom.
 * the class inherited from MThread.
 * this class contain a inner class: ChatRoomHandler.
 * data members in the class:
 * ChatRoomHandler* chatRoomHandler - an object of chatRoomHandler
 * bool active - indicate if the chatRoom is active
 * User* roomOwner
 * string roomName
 * vector<User*> usersInRoom - a vector of the Users in the room
 * MTCPListener listener - an object of MTCPListener
 */

#ifndef CHATROOM_H_
#define CHATROOM_H_

#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include "User.h"
#include "MThread.h"
#include "MTCPListener.h"
#include "TCPMessengerProtocol.h"

using namespace std;
namespace npl {

class ChatRoom: public MThread {
public:
	class ChatRoomHandler{
	public:
		virtual void onDisconnect(ChatRoom* chatRoom, User* disconnectingUser)=0;
		virtual void onClose(ChatRoom* chatRoom, vector<User*> usersVector)=0;
		virtual void onExit(ChatRoom* chatRoom, User* disconnectingUser)=0;
	};

private:

public:
	ChatRoomHandler* chatRoomHandler;
	bool active;
	User* roomOwner;
	string roomName;
	vector<User*> usersInRoom;
	MTCPListener listener;

	ChatRoom(ChatRoomHandler* chatRoomHandler, const string& roomName, User* roomOwner);
	void run();
	string getRoomOwner();
	string getUsersInRoom();
	void addUser(User* user);
	void userExitRoom(User* user);
	void notifyAllUsers(int command, const string& data, TCPSocket* socketSender);
	void exit(User* user);
    void close();
	virtual ~ChatRoom();
	vector<TCPSocket*> getPeersVector();
};

} /* namespace npl */

#endif /* CHATROOM_H_ */
