/*
 * TCPSocket.h
 *
 *  Created on: Mar 14, 2016
 *      Author: user
 */

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include "Socket.h"

using namespace std;
namespace npl{

class TCPSocket : public Socket {

private:
	TCPSocket(int connected_sock, struct sockaddr_in serverAddr,struct sockaddr_in peerAddr);
public:
	TCPSocket(int port); //SERVER SIDE
	TCPSocket(const string& ip, int port); //CLIENT SIDE
	TCPSocket* listenAndAccept();
	virtual ~TCPSocket();

};

}
#endif /* TCPSOCKET_H_ */
