/*
 * Dispatcher.cpp
 *
 *  Created on: May 27, 2016
 *      Author: user
 */

#include "Dispatcher.h"
#include <string>
#include <stdlib.h>


#define TIMEOUT 2

using namespace std;
namespace npl{


Dispatcher::Dispatcher() {
	//setRegisteredUsers();
	running = false;
	listener = new MTCPListener();
}

void Dispatcher::run(){
	int command = 0;
	string data = "";
	while (running) {
//		if (users.size() == 0) { //there is no peers to listen to
//			running = false;
//			break; // stop the thread
//		}
		TCPSocket* peer = listener->listen(TIMEOUT);
		if (peer != NULL) {
			User* userA = getUserByPeer(peer);
			command = 0;
			data.clear();
			TCPMessengerProtocol::readFromServer(command, data, peer);
			cout << "read command from peer: " << command << " " << data << endl;
			switch (command) {
			case OPEN_SESSION_WITH_PEER: {
				cout << "got open session command " << endl;
				openSession(userA, data);
				break;
			}
			case OPEN_OR_CONNECT_ROOM: {
				cout << "got open/ connect room command " << endl;
				openOrConnectRoom(userA, data);
				break;
			}
			case LIST_OF_USERS:{
				listOfUsers(peer);
				break;
			}
			case LIST_CONNECTED_USERS:{
				listConnectedUsers(peer);
				break;
			}
			case LIST_OF_ROOMS:{
				listOfRooms(peer);
				break;
			}
			case LIST_USERS_IN_ROOM:{
				listUsersInRoom(peer, data);
				break;
			}
			case EXIT:{
				exit(userA);
				break;
			}
			default:
				break;
			}
		}
	}
}



void Dispatcher::listAllUsers(){
	cout << "Users list: " << endl;
	map<string, string>::iterator itr = registeredUsers.begin();
	for (; itr != registeredUsers.end(); itr++) {
		cout << itr->first << endl;
	}
	cout << "***" << endl;
}


void Dispatcher::listConnectedUsers(){
	cout << "Connected users:" << endl;
	vector<User*>::iterator itr = loggedInUsers.begin();
	for (; itr != loggedInUsers.end(); itr++) {
		cout << (*itr)->userName << endl;
	}
	cout << "***" << endl;
}

void Dispatcher::listAllSessions(){
	if (sessions.size() > 0) {
		vector<Session*>::iterator itr = sessions.begin();
		for (; itr != sessions.end(); itr++) {
			cout << "[" << (*itr)->getUserA()->userName << ","
					<< (*itr)->getUserB()->userName << "]" << endl;
		}
	} else {
		cout << "NO users in session" << endl;
	}
	cout << "***" << endl;
}

void Dispatcher::listAllRooms(){
	if(chatRooms.size()>0){
		vector<ChatRoom*>::iterator itr = chatRooms.begin();
			for(;itr!=chatRooms.end();itr++){
				cout<< (*itr)->roomName<<endl;
			}
	}
	else {
		cout <<"NO rooms "<<endl;
	}
	cout << "***" << endl;
}

void Dispatcher::listUsersInRoom(string roomName){
	ChatRoom* cr = getRoomByName(roomName);
	if (cr==NULL){
		cout<<"ROOM does not exist"<<endl;
	}
	vector<User*>::iterator itr = cr->usersInRoom.begin();
		for(;itr!=cr->usersInRoom.end();itr++){
			cout<< (*itr)->userName<<endl;
		}
}


void Dispatcher::listOfUsers(TCPSocket* peer){
	string allUsers;
	map<string, string>::iterator itr = registeredUsers.begin();
	for (; itr != registeredUsers.end(); itr++) {
		allUsers += (itr)->first + "\n";
	}
	TCPMessengerProtocol::sendToServer(LIST_OF_USERS, allUsers, peer);
}

void Dispatcher::listConnectedUsers(TCPSocket* peer){
	string allConnectedUsers;
	vector<User*>::iterator itr = loggedInUsers.begin();
	for (; itr != loggedInUsers.end(); itr++) {
		allConnectedUsers += (*itr)->userName + "\n";
	}
	TCPMessengerProtocol::sendToServer(LIST_CONNECTED_USERS, allConnectedUsers, peer);
}

void Dispatcher::listOfRooms(TCPSocket* peer){
	string allRooms;
	if (chatRooms.size() > 0) {
		vector<ChatRoom*>::iterator itr = chatRooms.begin();
		for (; itr != chatRooms.end(); itr++) {
			allRooms += (*itr)->roomName + "\n";
		}
		TCPMessengerProtocol::sendToServer(LIST_OF_ROOMS, allRooms, peer);
	} else {
		TCPMessengerProtocol::sendToServer(LIST_OF_ROOMS, "No room\n", peer);
	}
}

void Dispatcher::listUsersInRoom(TCPSocket* peer, const string& data){
	bool flag = false;
	vector<ChatRoom*>::iterator itr = chatRooms.begin();
	for (; itr != chatRooms.end(); itr++) {
		if ((*itr)->roomName == data) {
			flag = true;
			TCPMessengerProtocol::sendToServer(LIST_USERS_IN_ROOM, (*itr)->getUsersInRoom(), peer);
			break;
		}
	}
	if(!flag){
		TCPMessengerProtocol::sendToServer(LIST_USERS_IN_ROOM, "Room does not exist\n", peer);
	}
}

void Dispatcher::addUser(User* user){
	if(std::find(users.begin(), users.end(), user) == users.end()) { // add user to vector if the user does not exist in the vector
		users.push_back(user);
		listener->add(user->tcpSocket);
	}
	if (std::find(loggedInUsers.begin(), loggedInUsers.end(), user) == loggedInUsers.end()) {
		loggedInUsers.push_back(user);
	}
	//cout<<"class Dispatcher: addUser"<<endl;
	if (!running) {
		running = true;
		start();
	}
}

void Dispatcher::addToRegistered(const string& userName, const string& pass){
	registeredUsers.insert(pair<string, string>(userName, pass));
}


void Dispatcher::removeUser(User* user){
	//users.erase(std::remove(users.begin(), users.end(), user), users.end()); // remove returns an iterator to the new end of that range
	vector<User*>::iterator itr = users.begin();
	for (; itr != users.end(); itr++) {
		if ((*itr) == user) {
			users.erase(itr);
			break;
		}
	}
	listener->remove(user->tcpSocket);
}
void Dispatcher::removeSession(Session* session){
	//sessions.erase(std::remove(sessions.begin(), sessions.end(), session), sessions.end()); // remove returns an iterator to the new end of that range
	vector<Session*>::iterator itr = sessions.begin();
	for (; itr != sessions.end(); itr++) {
		if ((*itr) == session) {
			sessions.erase(itr);
			break;
		}
	}
}

void Dispatcher::removeChatRoom(ChatRoom* chatRoom){
	//chatRooms.erase(std::remove(chatRooms.begin(), chatRooms.end(), chatRoom), chatRooms.end()); // remove returns an iterator to the new end of that range
	vector<ChatRoom*>::iterator itr = chatRooms.begin();
		for (; itr != chatRooms.end(); itr++) {
			if ((*itr) == chatRoom) {
				chatRooms.erase(itr);
				break;
			}
		}
}

void Dispatcher::openSession(User* user,const string& data){
	if(isLoggedIn(user)){
		User* userB = NULL;
		vector<User*>::iterator itr = loggedInUsers.begin();
		for(;itr!=loggedInUsers.end();itr++){
			if((*itr)->userName == data){
				userB = (*itr);
			}
		}
		if ((userB != NULL && user != userB) && !(std::find(users.begin(), users.end(), userB) == users.end())) {
			//cout<<"class Dispatcher: open session: userB is not NULL"<<endl;
			TCPMessengerProtocol::sendToServer(SESSION_ESTABLISHED, userB->userName + " " + userB->tcpSocket->fromAddr(), user->tcpSocket);
			TCPMessengerProtocol::sendToServer(SESSION_ESTABLISHED, user->userName + " " + user->tcpSocket->fromAddr(), userB->tcpSocket);
			this->removeUser(user);
			this->removeUser(userB);
			Session* session = new Session(this, user, userB);
			sessions.push_back(session);
			user->userStatus = IN_SESSION;
			userB->userStatus = IN_SESSION;
		} else {
			 TCPMessengerProtocol::sendToServer(SESSION_REFUSED,data,user->tcpSocket);
		}
	}
}

void Dispatcher::openOrConnectRoom(User* user,const string& roomName){
	if (isLoggedIn(user)) {
		if (isRoomExist(roomName)) {
			vector<ChatRoom*>::iterator itr = chatRooms.begin();
			for (; itr != chatRooms.end(); itr++) {
				if ((*itr)->roomName == roomName) {
					this->removeUser(user);
					(*itr)->addUser(user);
					user->userStatus = IN_ROOM;
					TCPMessengerProtocol::sendToServer(ENTER_TO_ROOM_SUCCESSES, roomName, user->tcpSocket);
					break;
				}
			}
		} else {
			this->removeUser(user);
			ChatRoom* newChatRoom = new ChatRoom(this, roomName, user);
			chatRooms.push_back(newChatRoom);
			user->userStatus = IN_ROOM;
			TCPMessengerProtocol::sendToServer(ENTER_TO_ROOM_SUCCESSES, roomName, user->tcpSocket);
		}
	}
}

bool Dispatcher::isLoggedIn(User* user){
	vector<User*>::iterator itr;
	itr = std::find(loggedInUsers.begin(), loggedInUsers.end(), user);
	if (itr != loggedInUsers.end())
		return true;
	else
		return false;
}

bool Dispatcher::isRoomExist(const string& roomName){
	vector<ChatRoom*>::iterator itr = chatRooms.begin();
	for (; itr != chatRooms.end(); itr++) {
		if ((*itr)->roomName == roomName) {
			return true;
		}
	}
	return false;
}

User* Dispatcher::getUserByPeer(TCPSocket* peer){
	vector<User*>::iterator itr = users.begin();
	for (; itr != users.end(); itr++) {
		if ((*itr)->tcpSocket == peer) {
			User* temp = (*itr);
			return temp;
		}
	}
	return NULL;
}

ChatRoom* Dispatcher::getRoomByName(const string& roomName){
	vector<ChatRoom*>::iterator itr = chatRooms.begin();
	for (; itr != chatRooms.end(); itr++) {
		if ((*itr)->roomName == roomName) {
			ChatRoom* temp = (*itr);
			return temp;
		}
	}
	return NULL;
}

void Dispatcher::exit(User* user){
	this->removeUser(user);
	if (isLoggedIn(user)) {
		//loggedInUsers.erase(std::remove(loggedInUsers.begin(), loggedInUsers.end(), user), loggedInUsers.end());
		vector<User*>::iterator itr = loggedInUsers.begin();
		for (; itr != loggedInUsers.end(); itr++) {
			if ((*itr) == user) {
				loggedInUsers.erase(itr);
				break;
			}
		}
	}
	cout << "Client " << user->userName << " disconnected" << endl;
}

void Dispatcher::onClose(Session* session, User* userA, User* userB){
	//sessions.erase(std::remove(sessions.begin(), sessions.end(), session), sessions.end()); // remove returns an iterator to the new end of that range
	vector<Session*>::iterator itr = sessions.begin();
	for (; itr != sessions.end(); itr++) {
		if ((*itr) == session) {
			sessions.erase(itr);
			break;
		}
	}
	this->addUser(userA);
	this->addUser(userB);
	//session->waitForThread();
}

void Dispatcher::onExit(Session* session, User* disconnectingUser, User* user){
	//sessions.erase(std::remove(sessions.begin(), sessions.end(), session), sessions.end()); // remove returns an iterator to the new end of that range
	vector<Session*>::iterator itr = sessions.begin();
	for (; itr != sessions.end(); itr++) {
		if ((*itr) == session) {
			sessions.erase(itr);
			break;
		}
	}
	this->addUser(user);
	//loggedInUsers.erase(std::remove(loggedInUsers.begin(), loggedInUsers.end(), disconnectingUser), loggedInUsers.end()); // remove returns an iterator to the new end of that range
	vector<User*>::iterator itr1 = loggedInUsers.begin();
	for (; itr1 != loggedInUsers.end(); itr1++) {
		if ((*itr1) == disconnectingUser) {
			loggedInUsers.erase(itr1);
			break;
		}
	}

	session->waitForThread();
	cout << disconnectingUser->userName << "disconnected" << endl;
	cout << "Session [" << disconnectingUser->userName << "," << user->userName << "] was closed" << endl;
}


void Dispatcher::onDisconnect(ChatRoom* chatRoom, User* disconnectingUser){
	this->addUser(disconnectingUser);
}

void Dispatcher::onClose(ChatRoom* chatRoom, vector<User*> usersVector){
	this->removeChatRoom(chatRoom);
	vector<User*>::iterator itr = usersVector.begin();
	for (; itr != usersVector.end(); itr++) {
		this->addUser((*itr));
	}
	chatRoom->waitForThread();
}

void Dispatcher::onExit(ChatRoom* chatRoom, User* disconnectingUser){
	//loggedInUsers.erase(std::remove(loggedInUsers.begin(), loggedInUsers.end(), disconnectingUser), loggedInUsers.end()); // remove returns an iterator to the new end of that range
	vector<User*>::iterator itr = loggedInUsers.begin();
	for (; itr != loggedInUsers.end(); itr++) {
		if ((*itr) == disconnectingUser) {
			loggedInUsers.erase(itr);
			break;
		}
	}
	cout<<"Client " << disconnectingUser->userName << " disconnected"<<endl;
}


void Dispatcher::close(){

}

Dispatcher::~Dispatcher(){

	// TODO Auto-generated destructor stub
}

void Dispatcher::setRegisteredUsers(){
	ifstream in;
	//map<string, string> usersAndPass;
	in.open("/home/user/Desktop/workspace2/users.txt", ios::in);
	if(in){
		string userName;
		string password;
		while (std::getline(in, userName) && getline(in, password)) {
			//usersAndPass.insert(pair<string, string>(userName, password));
			registeredUsers.insert(pair<string, string>(userName, password));
		}
	}
	in.close();
}

} /* namespace npl */
