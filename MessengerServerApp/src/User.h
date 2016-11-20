/*
 * User.h
 *
 *  Created on: May 28, 2016
 *      Author: user
 */

#ifndef USER_H_
#define USER_H_
#include <string>
#include <stdio.h>
#include "TCPSocket.h"
#include "Status.h"

using namespace std;
namespace npl {

class User {
public:
	string password;
	string roomName;
	TCPSocket* tcpSocket;
	string userName;
	Status userStatus;
	User();
	User(string userName, string password, TCPSocket* tcpSocket);
	User(const User& user);

	virtual ~User();
};

} /* namespace npl */

#endif /* USER_H_ */
