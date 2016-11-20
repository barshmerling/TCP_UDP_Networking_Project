/*
 * Socket.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: user
 */
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "Socket.h"

using namespace npl;

int Socket::bind(int port) {
	bzero((char*) &localAddr, sizeof(localAddr));

	//sets the sin address
	localAddr.sin_family = (short) AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(port);

	//bind the socket on the specified address
	int rc = ::bind(socket_fd, (struct sockaddr *) &localAddr, sizeof(localAddr));
	if (rc < 0) {
		cerr << "Error naming channel" << endl;
		::close(socket_fd);
	}
	cout << "Server is alive and waiting for socket connection from client."
			<< endl;
	return rc;
}

int Socket::connect(const string& ip, int port){
	// set the peer adress to connect to
	bzero(&remoteAddr, sizeof(remoteAddr));
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	remoteAddr.sin_port = htons(port);

	//connect the socket to the peer server
	int rc = ::connect(socket_fd, (struct sockaddr*) &remoteAddr, sizeof(remoteAddr));
	if (rc < 0) {
		cerr << "connection failed" << endl;
		::close(socket_fd);
	}
	return rc;
}

string Socket::fromAddr() {
	char addr[100];
	::sprintf(addr, "%s:%d", inet_ntoa(remoteAddr.sin_addr), remoteAddr.sin_port);
	return addr;
//	string str = inet_ntoa(remoteAddr.sin_addr);
//	str.append(":");
//	char buff[10];
//	sprintf(buff, "%d", ntohs(remoteAddr.sin_port));
//	str.append(buff);
//	return str;
}

string Socket::getIpRemoteAddr() {
	return inet_ntoa(remoteAddr.sin_addr);
}

int Socket::getPortRemoteAddr() {
	return ntohs(remoteAddr.sin_port);
}


