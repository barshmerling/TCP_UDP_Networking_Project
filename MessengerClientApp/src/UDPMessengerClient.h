/*
 * UDPMessengerClient.h
 *
 *  Created on: May 27, 2016
 *      Author: user
 */
/**
 * this class represents the receive  message on UDP
 * data members in the class:
 * UDPSocket* udpSocket - an object of UDPSocket
 * bool running - true as long as the method run shod be run
 * methods in the class:
 * void run() - open a new thread and receive message on UDP
 * void stopRunning() - the data member running change to false
 */

#include "MThread.h"
#include "UDPSocket.h"
#include <string.h>


#ifndef UDPMESSENGERCLIENT_H_
#define UDPMESSENGERCLIENT_H_
#define SIZE  256

using namespace std;
namespace npl {

class UDPMessengerClient :public MThread {
UDPSocket* udpSocket;
bool running;

public:

	UDPMessengerClient(UDPSocket* udpSocket);
	void run();
	void stopRunning();
	virtual ~UDPMessengerClient();
};

} /* namespace npl */

#endif /* UDPMESSENGERCLIENT_H_ */
