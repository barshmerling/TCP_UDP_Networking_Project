/*
 * Session.h
 *
 *  Created on: May 28, 2016
 *      Author: user
 */
/**
 * this class represent a session between two peers.
 * the class inherited from MThread.
 * this class contain a inner class: SessionHandler.
 * data members in the class:
 * SessionHandler* handler - an object of SessionHandler
 * bool active - indicate if the session is active
 * User *userA - an object of User
 * User *userB - an object of User
 */

#ifndef SESSION_H_
#define SESSION_H_

#include "User.h"
#include <vector>
#include "MThread.h"
#include "TCPMessengerProtocol.h"
#include "MTCPListener.h"

using namespace std;
namespace npl {

class Session:public MThread {
public:
	class SessionHandler{
	public:
		virtual void onClose(Session* session, User* userA, User* userB)=0;
		virtual void onExit(Session* session, User* disconnectingUser, User* userA)=0;
	};

private:
	SessionHandler* handler;
	bool active;
	User *userA;
	User *userB;

public:
	Session(SessionHandler* sessionHandler, User *user1,User *user2);
	void run();
	User* getUserA();
	User* getUserB();
	void close();
	void closeByExit(User* disconnectingUser);
	virtual ~Session();
};

} /* namespace npl */

#endif /* SESSION_H_ */
