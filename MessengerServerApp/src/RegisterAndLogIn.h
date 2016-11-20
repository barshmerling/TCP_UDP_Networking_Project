/*
 * RegisterAndLogIN.h
 *
 *  Created on: May 27, 2016
 *      Author: user
 */
/**
 * this class handle the client's apply of register and log-in.
 * the class inherited from MThread.
 * the class contain a inner class: Handler.
 * 	the inner class contain the  pure methods:
 * 	virtual void onLogin(RegisterAndLogIn* reglogin,User* user) - transfer the user to the manage of the dispatcher
 * 	virtual void onRegister(RegisterAndLogIn* reglogin,const string& name,const string& pass) - add the registered-user to the vector of registeredUsers on the dispatcher
 * data members in the class:
 * Handler* handler - an object of handler
 * 	vector<TCPSocket*> peers - a vector of TCPSocket
 * 	map<string,string> registeredUsers - mapping <user name,pass>
 * 	MTCPListener* listener - an object of handler MTCPListener
 * 	string path - the path of the file
 * 	bool running
 */
#ifndef REGISTERANDLOGIN_H_
#define REGISTERANDLOGIN_H_

//#include "File.h"
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <fstream>// std::ofstream
#include <sstream>
#include <map>
#include "TCPMessengerProtocol.h"
#include "User.h"
#include "MThread.h"
#include "MTCPListener.h"


using namespace std;

namespace npl {

class RegisterAndLogIn:public MThread{

public:
	class Handler{
	public:
	virtual void onLogin(RegisterAndLogIn* reglogin,User* user)=0;
	virtual void onRegister(RegisterAndLogIn* reglogin,const string& name,const string& pass)=0;
	};

private:
	Handler* handler;
	vector<TCPSocket*> peers;
	map<string,string> registeredUsers; // mapping <user name,pass>
	MTCPListener* listener;
	string path;
	bool running;

public:
	RegisterAndLogIn(Handler* handler, string _path);
	void addPeerToListen(TCPSocket* socket);
	void login(User* user);
	void registration(const string& user,const string& userPassword);
	bool InsertUserToFile(const string& user,const string& userPassword);
	map<string,string> loadUsersFromFile();
	void run();
	virtual ~RegisterAndLogIn();

};
} /* namespace npl */

#endif /* REGISTERANDLOGIN_H_ */
