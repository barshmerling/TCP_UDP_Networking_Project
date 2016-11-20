/*
 * UDPSocket.h
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include "FileInterface.h"

using namespace std;
namespace npl{

class Socket: public FileInterface{

protected:
	struct sockaddr_in  localAddr;
	struct sockaddr_in  remoteAddr;

public:
	int bind(int port);
	int connect(const string& ip, int port);
	virtual string fromAddr();
	virtual string getIpRemoteAddr();
	virtual int getPortRemoteAddr();
};
}
#endif /* SOCKET_H_ */
