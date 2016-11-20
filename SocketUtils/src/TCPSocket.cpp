/*
 * TCPSocket.cpp
 *
 *  Created on: Mar 14, 2016
 *      Author: user
 */

#include "TCPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

using namespace npl;

TCPSocket::TCPSocket(int port){
	//open TCP socket
	socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0){
		cerr<<"Error opening channel"<<endl;
		::close(socket_fd);
	}
	if(bind(port) < 0){
		cerr << "Error naming channel" << endl;
	}
}

TCPSocket::TCPSocket(const string& ip, int port){
	//open TCP socket - client side
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0){
		cerr<<"Error opening channel"<<endl;
		::close(socket_fd);
	}
	if(connect(ip, port) < 0){
		cerr << "connection failed" << endl;
	}
}

TCPSocket::TCPSocket(int connected_sock,sockaddr_in localAddr,sockaddr_in remoteAddr){
	this->socket_fd = connected_sock;
	this->remoteAddr = remoteAddr;
	this->localAddr = localAddr;
}

TCPSocket* TCPSocket:: listenAndAccept(){
	//listen to the socket
	listen(socket_fd,1);
	//call accept to the socket
	unsigned int length = sizeof(remoteAddr);
	int connect_sock = accept(socket_fd,(struct sockaddr*)&remoteAddr,&length);
	if (connect_sock <=0){
		cerr<<"failed to accept connection;"<<endl;
		return NULL;
	}
	// return new TCPSocket object holding the new secondary server socket returned from the accept call
	return new TCPSocket(connect_sock,localAddr,remoteAddr);
}

TCPSocket::~TCPSocket(){

}



