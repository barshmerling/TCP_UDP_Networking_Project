/*
 * TCPMessengerClient.cpp
 *
 *  Created on: May 27, 2016
 *      Author: user
 */
#include "TCPMessengerClient.h"

using namespace std;
namespace npl{

TCPMessengerClient::TCPMessengerClient() {
	this->cStatus = DISCONNECTED;
	this->roomName = "";
	this->peersInRoom = new map<string,string>();
	this->peerInSession = NULL;
	this->udpSocket = NULL;
}

void TCPMessengerClient::run() {
	while (cStatus != DISCONNECTED) {
		int cmd;
		string str;
		TCPMessengerProtocol::readFromServer(cmd, str, myServer);
		if (cmd > 0) {
			if (cmd == LOG_IN_SUCCESSES) {
				cout<<"LOG_IN_SUCCESSES"<<endl;
				std::istringstream splitter(str);
				string user;
				string ip;
				int port;
				splitter >> user;
				std::getline(splitter, ip, ':');
				splitter >> port;

				this->cStatus = LOGGED_IN;
				this->clientName = user;
				this->myPort = port;
				cout << "User: " << user << " Successfully Logged In" << endl;
			} else if (cmd == REGISTER_SUCCESSES) {
				std::istringstream splitter(str);
				string user;
				string ip;
				int port;
				splitter >> user;
				std::getline(splitter, ip, ':');
				splitter >> port;
				//this->cStatus = LOGGED_IN;
				this->clientName = user;
				this->myPort = port;
				cout << "User: " << user << " Successfully Registered" << endl;
			} else if (cmd == LOG_IN_FAILED) {
				cout << "Please check your UserName & Password " << endl;
			} else if (cmd == REGISTER_FAILED) {
				cout << "UserName already Exist " << endl;
			} else if (cmd == SESSION_ESTABLISHED) {
				//input format: <user> <ip>:<port>
				this->cStatus = IN_SESSION;
				std::istringstream splitter(str);
				string peerUser;
				string peerIpAndPort;
				splitter >> peerUser;
				splitter >> peerIpAndPort;
				cout << "Session Established Successfully" << endl;
				this->peerInSession = new pair<string, string>(peerUser, peerIpAndPort);
				this->udpSocket = new UDPSocket(this->myPort);
				this->udpMessengerClient = new UDPMessengerClient(this->udpSocket);
			} else if (cmd == ENTER_TO_ROOM_SUCCESSES) {
				this->cStatus = IN_ROOM;
				this->roomName = str;
				this->udpSocket = new UDPSocket(this->myPort);
				this->udpMessengerClient = new UDPMessengerClient(this->udpSocket);
				cout << " Entrance room: " << str << " Successfully" << endl;
			} else if (cmd == SESSION_REFUSED) {
				cout << "Session Refused" << endl;
			} else if (cmd == CLOSE_SESSION_WITH_PEER) {
				cout << "Close Session with: " << this->peerInSession->first << endl;
				delete this->peerInSession;
				if (this->udpMessengerClient != NULL) {
					this->udpMessengerClient->stopRunning();
				}
				this->udpSocket->close();
				this->cStatus = LOGGED_IN;
			} else if (cmd == CLIENT_EXITED_FROM_ROOM) {
				for (map<string, string>::iterator itr = peersInRoom->begin(); itr != peersInRoom->end(); ++itr) {
					if (itr->first == str) {
						this->peersInRoom->erase(itr->first);
						cout << "Client: " << str << " left the room" << endl;
						break;
					}
				}
			} else if (cmd == CLIENT_ENTERED_ROOM) {
				std::istringstream splitter(str);
				string peerUser;
				string peerIpAndPort;
				splitter >> peerUser;
				splitter >> peerIpAndPort;
				this->peersInRoom->insert(make_pair(peerUser, peerIpAndPort));
				cout << "User " << str << "entered to the room" << endl;
			} else if (cmd == MANAGER_LEAVE_ROOM) {
				if (this->udpMessengerClient != NULL) {
					this->udpMessengerClient->stopRunning();
				}
				this->udpSocket->close();
				this->cStatus = LOGGED_IN;
				cout << "You close the chat room" << endl;
			} else if (cmd == ROOM_CLOSED_BY_MANAGER) {
				this->peersInRoom->clear();
				if (this->udpMessengerClient != NULL) {
					this->udpMessengerClient->stopRunning();
				}
				this->udpSocket->close();
				this->cStatus = LOGGED_IN;
				cout << "Chat room closed by manager" << endl;
			} else if (cmd == EXIT_ROOM_BACK) {
				this->peersInRoom->clear();
				if (this->udpMessengerClient != NULL) {
					this->udpMessengerClient->stopRunning();
				}
				this->udpSocket->close();
				this->cStatus = LOGGED_IN;
				cout << "You exited from the room" << endl;
			} else if (cmd == LIST_OF_USERS) {
				cout << "Registered Users:\n" << str << endl;
			} else if (cmd == LIST_CONNECTED_USERS) {
				cout << "Connected Users:\n" << str << endl;
			} else if (cmd == LIST_OF_ROOMS) {
				cout << "Rooms:\n" << str << endl;
			} else if (cmd == LIST_USERS_IN_ROOM) {
				cout << "Room Users:\n" << str << endl;
			}
		} else {
			cout << "Server Disconnected!!" << endl;
			this->close();
		}
	}
}


void TCPMessengerClient::connect(const string& ip) {
	if (this->cStatus == DISCONNECTED) {
		cout << "Connecting to server " << ip << endl << flush;
		myServer = new TCPSocket(ip, MSNGR_PORT);
		if (myServer != NULL) {
			this->cStatus = CONNECTED;
			start();
		}
		cout << "Opening new client socket" << endl;
	} else {
		cout << "You are already connected to the server" << endl;
	}
}

void TCPMessengerClient::registr(const string& userName, const string& password) {
	if (this->cStatus == CONNECTED){
		TCPMessengerProtocol::sendToServer(REGISTER, userName + " " + password,myServer);
	} else {
		cout << "You are not connect to the server" << endl;
	}

}

void TCPMessengerClient::logIn(const string& userName, const string& password) {
	cout<<"class client: login: before this->cStatus == CONNECTED..."<<endl;
	if (this->cStatus == CONNECTED){
		cout<<"class client: login: this->cStatus == CONNECTED..."<<endl;
		TCPMessengerProtocol::sendToServer(LOG_IN, userName+" "+password, myServer);
	} else {
		cout << "You are not connect to the server" << endl;
	}
}

void TCPMessengerClient::currentStatus(){
    if (cStatus == CONNECTED)
        cout<<"Client is connected to server"<<endl;
    else if (cStatus == DISCONNECTED)
        cout<<"Client is not connected to the server"<<endl;
    else if (cStatus == LOGGED_IN)
        cout<<"Client is logged in to server"<<endl;
    else if (cStatus == IN_SESSION)
        cout<<"Client in session"<<endl;
    else if (cStatus == IN_ROOM)
        cout <<"Client in room "<<this->roomName<<endl;
}

void TCPMessengerClient::closeRoom(string roomName){
	if (cStatus == IN_ROOM) {
		TCPMessengerProtocol::sendToServer(CLOSE_ROOM, this->roomName, myServer);
	} else {
		cout << "You are not in the room" << endl;
	}
}

void TCPMessengerClient::listOfUsers() {
    if (cStatus == LOGGED_IN || cStatus == IN_ROOM || cStatus== IN_SESSION){
        TCPMessengerProtocol::sendToServer(LIST_OF_USERS," ",myServer);
    }
    else{
        cout<<"You are not logged in"<<endl;
    }
}

void TCPMessengerClient::listConnectedUsers() {
    if (cStatus == LOGGED_IN || cStatus == IN_ROOM || cStatus== IN_SESSION){
        TCPMessengerProtocol::sendToServer(LIST_CONNECTED_USERS," ",myServer);
    }
    else{
        cout<<"You are not logged in"<<endl;
    }
}

void TCPMessengerClient::ListOfRooms() {
    if (cStatus == LOGGED_IN || cStatus == IN_ROOM || cStatus== IN_SESSION){
        TCPMessengerProtocol::sendToServer(LIST_OF_ROOMS," ",myServer);
    }
    else{
        cout<<"You are not logged in"<<endl;
    }
}

void TCPMessengerClient::listUsersInRoom(string roomName) {
    if (cStatus == LOGGED_IN || cStatus == IN_ROOM || cStatus== IN_SESSION){
        TCPMessengerProtocol::sendToServer(LIST_USERS_IN_ROOM,roomName,myServer);
    }
    else{
		cout << "You are not logged in" << endl;
    }
}

void TCPMessengerClient::openSession(const string& clientName) {
	if (cStatus == LOGGED_IN){
		TCPMessengerProtocol::sendToServer(OPEN_SESSION_WITH_PEER, clientName, myServer);
	}
	else if (cStatus == IN_SESSION)
		cout << "You are already in a session " << endl;
	else if (cStatus == IN_ROOM)
		cout << "You are already in a room " << endl;
	else
		cout << "You are not logged in" << endl;
}

void TCPMessengerClient::openOrConnectRoom(const string& roomName) {
	if (cStatus == LOGGED_IN)
		TCPMessengerProtocol::sendToServer(OPEN_OR_CONNECT_ROOM, roomName, myServer);
	else if (cStatus == IN_SESSION)
		cout << "You are already in a session " << endl;
	else if (cStatus == IN_ROOM)
		cout << "You are already in a room " << endl;
	else
		cout << "You are not logged in" << endl;
}

//------>> UDP
 void TCPMessengerClient::send(const string& msg){
	string peerIp;
	int peerPort;
	if (cStatus == IN_SESSION) {
		istringstream splitter(this->peerInSession->second);
		getline(splitter, peerIp, ':');
		splitter >> peerPort;
		//send the other peer the message
		this->udpSocket->sendTo(">[" + clientName + "] " + msg, peerIp, peerPort);
	} else if (cStatus == IN_ROOM) {
		istringstream splitter;
		for (map<string, string>::iterator iter = peersInRoom->begin(); iter != peersInRoom->end(); ++iter) {
			splitter.clear();
			splitter.str(iter->second);
			getline(splitter, peerIp, ':');
			splitter >> peerPort;
			this->udpSocket->sendTo(">[" + clientName + "] " + msg, peerIp, peerPort);
		}
	} else {
		cout << "You are not in session/room" << endl;
	}
 }

void TCPMessengerClient::exitRoomOrCloseSession() {
	if (cStatus == IN_SESSION) {
		TCPMessengerProtocol::sendToServer(CLOSE_SESSION_WITH_PEER, " ",myServer);
	} else if (cStatus == IN_ROOM) {
		cout<<"class client: metod exitRoomOrCloseSession: user in room"<<endl;
		TCPMessengerProtocol::sendToServer(EXIT_ROOM, " " ,myServer);
	} else {
		cout << "You not in session/room" << endl;
	}
}

void TCPMessengerClient::disconnect() {
	if (cStatus != DISCONNECTED) {
		TCPMessengerProtocol::sendToServer(EXIT, " ", myServer);
		close();
		cout << "You disconnected from server" << endl;
	} else
		cout << "You are already disconnected" << endl;
}

void TCPMessengerClient::exit() {
	if (cStatus != DISCONNECTED) {
		TCPMessengerProtocol::sendToServer(EXIT, " ", myServer);
	}
	close();
	cout << "You  disconnected from server" << endl;
}

void TCPMessengerClient::close(){
	cout << "Closing All" << endl;
	this->roomName = "";
	this->clientName = "";
	this->myPort = 0;
	if (this->peerInSession != NULL) {
		delete this->peerInSession;
	}
	if (this->peersInRoom != NULL) {
		this->peersInRoom->clear();
	}
	if (this->udpMessengerClient != NULL) {
		this->udpMessengerClient->stopRunning();
		delete this->udpMessengerClient;
	}
	if (this->udpSocket != NULL) {
		this->udpSocket->close();
	}
	if (this->cStatus != DISCONNECTED) {
		this->cStatus = DISCONNECTED;
	}
	if (this->myServer != NULL) {
		this->myServer->close();
	}
	cout << "Done!" << endl;

	::exit(0);
}

TCPMessengerClient::~TCPMessengerClient() {

}

} /* namespace npl */
