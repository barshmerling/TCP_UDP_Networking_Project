/*
 * Session.cpp
 *
 *  Created on: May 28, 2016
 *      Author: user
 */

#include "Session.h"

namespace npl{
Session::Session(SessionHandler* sessionHandler, User *user1,User *user2) {
	this->handler = sessionHandler;
	this->userA = user1;
	this->userB = user2;
	this->active = true;
	start();
}
void Session::run(){
    MTCPListener listener;
    User* sender;
    User* reciver;

    sender = NULL;
    reciver = NULL;

    listener.add(userA->tcpSocket);
    listener.add(userB->tcpSocket);

     int command = 0;
     string data = "";

     while(active){
		TCPSocket* tcpSocket = listener.listen(0);
		sender = NULL;
		reciver = NULL;

		if (tcpSocket == userA->tcpSocket) {
			sender = userA;
			reciver = userB;
		} else {
			sender = userB;
			reciver = userA;
		}
		if (sender != NULL && sender->tcpSocket != NULL) {
    		 TCPMessengerProtocol::readFromServer(command,data,sender->tcpSocket);
    		 switch(command){
    		 case CLOSE_SESSION_WITH_PEER:{
    			 TCPMessengerProtocol::sendToServer(command,reciver->userName,sender->tcpSocket);
    			 TCPMessengerProtocol::sendToServer(command,sender->userName,reciver->tcpSocket);
    			 close();
    			 break;
    		 }
    		 case EXIT:{
    			 TCPMessengerProtocol::sendToServer(CLOSE_SESSION_WITH_PEER,sender->userName,reciver->tcpSocket);
    			 closeByExit(sender);
    			 break;
    		 }
    		 default:{
    			 break;
    		 }
    		}
    	 }
     }

}

User* Session::getUserA(){
	return this->userA;
}
User* Session::getUserB(){
	return this->userB;
}

void Session::close(){
	active = false;
	this->handler->onClose(this,userA,userB);
}

void Session::closeByExit(User* disconnectingUser){
	active = false;
	if(disconnectingUser == this->userA){
		this->handler->onExit(this, userA, userB);
	}
	else{
		this->handler->onExit(this, userB, userA);
	}
}


Session::~Session() {
	// TODO Auto-generated destructor stub
}

}
