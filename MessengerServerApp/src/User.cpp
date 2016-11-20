/*
 * User.cpp
 *
 *  Created on: May 28, 2016
 *      Author: user
 */

#include "User.h"

namespace npl {

User::User(){
	this->userName = " ";
	this->password = " ";
	this->tcpSocket = NULL;
}

User::User(string userName, string password, TCPSocket* tcpSocket) {
	this->userName = userName;
	this->password = password;
	this->tcpSocket = tcpSocket;
}

User::User(const User& user){
	this->password = user.password;
	//this->roomName = user.roomName;
	this-> tcpSocket = user.tcpSocket;
	this->userName = user.userName;
	this->userStatus = user.userStatus;
}

User::~User() {
	// TODO Auto-generated destructor stub
}

} /* namespace npl */
