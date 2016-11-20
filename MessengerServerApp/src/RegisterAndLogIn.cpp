/*
 * RegisterAndLogIN.cpp
 *
 *  Created on: May 27, 2016
 *      Author: user
 */


#include "RegisterAndLogIn.h"

using namespace std;
namespace npl{

RegisterAndLogIn::RegisterAndLogIn(Handler* handler,string _path) {
	this->handler = handler;
	this->listener = new MTCPListener();
	this->running = true;
	this->path = _path;
	this->start();
}

void RegisterAndLogIn::addPeerToListen(TCPSocket* socket){
	peers.push_back(socket);
	this->listener->add(socket);
}

bool RegisterAndLogIn::InsertUserToFile(const string& userName,const string& userPassword){
	for (map<string, string>::iterator itr = this->registeredUsers.begin(); itr != this->registeredUsers.end(); ++itr) {
		if (itr->first == userName) {
			cout << "ERROR: The user " << "'" << itr->first << "'"<< " is already exist" << endl;
			return false;
		}
	}
	this->registeredUsers.insert(pair<string, string>(userName, userPassword));

	ofstream out;
	out.open("/home/user/Desktop/workspace2/users.txt",ios::app | ios::out);
	out << userName << endl;
	out.flush();
	out << userPassword << endl;
	out.flush();
	out.close();
	return true;
}

map<string,string> RegisterAndLogIn::loadUsersFromFile(){
	ifstream in;
	in.open("/home/user/Desktop/workspace2/users.txt", ios::in);
	if(in){
		string userName;
		string password;
		while (std::getline(in, userName) && getline(in, password)) {
			registeredUsers.insert(pair<string, string>(userName, password));
		}
	}
	in.close();
	return this->registeredUsers;
}

void RegisterAndLogIn::run() {
	int command;
	string data;
	while (running) {
//		if (peers.size()== 0){
//			running = false;
//			break;
//		}
		TCPSocket* peer = listener->listen(2);
		if (peer != NULL) {
			command = 0;
			data.clear();
			TCPMessengerProtocol::readFromServer(command, data, peer);
			switch (command) {
			case LOG_IN: {
				bool loginSuccess = false;
				std::istringstream splitter(data);
				string peerUser;
				string peerPassword;
				splitter >> peerUser;
				splitter >> peerPassword;

				for (map<string, string>::iterator itr = registeredUsers.begin(); itr != registeredUsers.end(); ++itr) {
					if ((itr->first == peerUser) && (itr->second == peerPassword)) {
						User* user = new User(peerUser, peerPassword, peer);
						user->userStatus = LOGGED_IN;
						TCPMessengerProtocol::sendToServer(LOG_IN_SUCCESSES, peerUser + " " + peer->fromAddr(), peer);
						loginSuccess = true;
						this->login(user);
					}
				}
				if (!loginSuccess) {
					TCPMessengerProtocol::sendToServer(LOG_IN_FAILED, peerUser + " " + peer->fromAddr(), peer);
				}

				break;
			}
			case REGISTER: {
				bool alreadyExist = false;
				std::istringstream splitter(data);
				string peerUser;
				string peerPassword;
				splitter >> peerUser;
				splitter >> peerPassword;

				for (map<string, string>::iterator itr = registeredUsers.begin(); itr != registeredUsers.end(); ++itr) {
					if (itr->first == peerUser) {
						TCPMessengerProtocol::sendToServer(REGISTER_FAILED, data, peer);
						alreadyExist = true;
						break;
					}
				}

				if (!alreadyExist) {
					//write the new user to the file and check if succeeded writing
					if (this->InsertUserToFile(peerUser, peerPassword)) {
						//insert peer to registered map
						registeredUsers.insert(pair<string, string>(peerUser, peerPassword));
						User* user = new User(peerUser, peerPassword, peer);
						user->userStatus = CONNECTED;
						TCPMessengerProtocol::sendToServer(REGISTER_SUCCESSES, peerUser + " " + peer->fromAddr(), peer);
						this->registration(peerUser, peerPassword);
					}
				}
				break;
			}
			default: {
				break;
			}
			}
		}
	}
}

void RegisterAndLogIn::login(User* user){
	peers.erase(std::remove(peers.begin(), peers.end(), user->tcpSocket), peers.end());
	listener->remove(user->tcpSocket);
	this->handler->onLogin(this,user);
}

void RegisterAndLogIn::registration(const string& user,const string& userPassword){
	this->handler->onRegister(this,user, userPassword);
}


RegisterAndLogIn::~RegisterAndLogIn() {
	// TODO Auto-generated destructor stub
}

} /* namespace npl */
