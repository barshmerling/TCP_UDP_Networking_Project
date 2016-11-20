/*
 * ChatRoom.cpp
 *
 *  Created on: May 27, 2016
 *      Author: user
 */

#include "ChatRoom.h"
using namespace std;
namespace npl{

ChatRoom::ChatRoom(ChatRoomHandler* chatRoomHandler, const string& roomName, User* roomOwner) {
	this->chatRoomHandler = chatRoomHandler;
	this->roomOwner = roomOwner;
	this->roomName = roomName;
	this->usersInRoom.push_back(roomOwner);
	this->active = true;
	start();
}

void ChatRoom::run(){
	cout << "Room " << roomName << "is open" << endl;
	listener.add(this->roomOwner->tcpSocket);
	int command = 0;
	string data = "";
	User* sender = NULL;

	while (active) {
		listener.add(getPeersVector());
		TCPSocket* tcpSocket = listener.listen(2);
		sender = NULL;
		for (vector<User*>::iterator itr = usersInRoom.begin(); itr != usersInRoom.end(); itr++) {
			if ((*itr)->tcpSocket == tcpSocket) {
				sender = (*itr);
			}
		}
		if (sender != NULL && sender->tcpSocket != NULL) {
			TCPMessengerProtocol::readFromServer(command, data, sender->tcpSocket);
			switch (command) {
			case EXIT_ROOM: { // client send cs
				this->userExitRoom(sender);
				break;
			}
			case CLOSE_ROOM: { // client send cr
				this->userExitRoom(sender);
				break;
			}
			case EXIT: {
				this->exit(sender);
				break;
			}
			default: {
				break;
			}
			}
		}
	}
}

string ChatRoom::getRoomOwner(){
	return roomOwner->userName;
}

string ChatRoom::getUsersInRoom(){
	string allUsersInRoom;
	vector<User*>::iterator itr = usersInRoom.begin();
	for (; itr != usersInRoom.end(); itr++) {
		allUsersInRoom += (*itr)->userName + "\n";
	}
	return allUsersInRoom;
}

void ChatRoom::addUser(User* user){
	usersInRoom.push_back(user);
	for (vector<User*>::iterator itr = usersInRoom.begin();
			itr != usersInRoom.end(); itr++) {
		cout << "user: " << (*itr)->userName << " " << (*itr)->tcpSocket->fromAddr() << endl;
	}
	listener.add(user->tcpSocket);
	//notify to all Users in room that new user join the room
	if (usersInRoom.size() > 1) {
		this->notifyAllUsers(CLIENT_ENTERED_ROOM, user->userName + " " + user->tcpSocket->fromAddr(), user->tcpSocket);
	}
	//notify the new user- who joined the room - about all the usersInRoom
	vector<User*>::iterator itr = usersInRoom.begin();
	for (; itr != usersInRoom.end(); itr++) {
		if ((*itr) != user) {
			TCPMessengerProtocol::sendToServer(CLIENT_ENTERED_ROOM, (*itr)->userName + " " + (*itr)->tcpSocket->fromAddr(), user->tcpSocket);
		}
	}
}

void ChatRoom::userExitRoom(User* user){
	if (user == roomOwner){
		//the manager send to himself
		TCPMessengerProtocol::sendToServer(MANAGER_LEAVE_ROOM, user->tcpSocket->fromAddr(), user->tcpSocket);
		this->notifyAllUsers(ROOM_CLOSED_BY_MANAGER, " ", user->tcpSocket);
		this->close();
	} else {
		TCPMessengerProtocol::sendToServer(EXIT_ROOM_BACK, user->userName, user->tcpSocket);
		vector<User*>::iterator itr = usersInRoom.begin();
		for (; itr != usersInRoom.end(); itr++) {
			if ((*itr) == user) {
				usersInRoom.erase(itr);
				break;
			}
		}

		listener.remove(user->tcpSocket);
		this->notifyAllUsers(CLIENT_EXITED_FROM_ROOM, user->userName, user->tcpSocket);
		this->chatRoomHandler->onDisconnect(this, user);
		user->userStatus = LOGGED_IN;
	}
}

void ChatRoom::notifyAllUsers(int command, const string& data, TCPSocket* socketSender){
	vector<User*>::iterator itr = usersInRoom.begin();
	for (; itr != usersInRoom.end(); itr++) {
		if ((*itr)->tcpSocket != socketSender) {
			TCPMessengerProtocol::sendToServer(command, data, (*itr)->tcpSocket);
		}
	}
}

void ChatRoom::exit(User* user){
	if (user == roomOwner) {
		this->notifyAllUsers(ROOM_CLOSED_BY_MANAGER, " ", user->tcpSocket);
		this->chatRoomHandler->onExit(this, user);
		this->close();
	} else {
		this->notifyAllUsers(CLIENT_EXITED_FROM_ROOM, user->userName, user->tcpSocket);
		vector<User*>::iterator itr = usersInRoom.begin();
		for (; itr != usersInRoom.end(); itr++) {
			if ((*itr) == user) {
				usersInRoom.erase(itr);
				break;
			}
		}

		listener.remove(user->tcpSocket);
		this->chatRoomHandler->onExit(this, user);
	}
}

void ChatRoom::close(){
	active = false;
	vector<User*>::iterator itr = usersInRoom.begin();
	for (; itr != usersInRoom.end(); itr++) {
		listener.remove((*itr)->tcpSocket);
	}
}

ChatRoom::~ChatRoom() {
}

vector<TCPSocket*> ChatRoom::getPeersVector(){
	vector<TCPSocket*> peers;
	vector<User*>::iterator itr = usersInRoom.begin();
	for (; itr != usersInRoom.end(); itr++){
		peers.push_back((*itr)->tcpSocket);
	}
	return peers;
}

} /* namespace npl */
