/*
 * MTCPListener.h
 *
 *  Created on: Apr 3, 2016
 *      Author: user
 */

#ifndef MTCPLISTENER_H_
#define MTCPLISTENER_H_

#include "TCPSocket.h"
#include <iostream>
#include <stdio.h>
#include <algorithm>// std::remove
#include <vector>

using namespace std;

namespace npl {

class MTCPListener {
private:
	vector<TCPSocket*> sockets;

public:
	/**
	 * add sockets to listen to
	 */
	void add(TCPSocket* tcpSocket);
	void add(vector<TCPSocket*> vectorSockets);
	/**
	 * listen to a previously added socket, returns the socket ready for reading
	 */
	TCPSocket* listen(int timeout);
	void remove(TCPSocket* socket);

};

} /* namespace npl */

#endif /* MTCPLISTENER_H_ */
