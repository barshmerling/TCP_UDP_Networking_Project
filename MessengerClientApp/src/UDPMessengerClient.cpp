/*
 * UDPMessengerClient.cpp
 *
 *  Created on: May 27, 2016
 *      Author: user
 */

#include "UDPMessengerClient.h"

namespace npl {

UDPMessengerClient::UDPMessengerClient(UDPSocket* udpSocket) {
	this->udpSocket = udpSocket;
	running = true;
	start();
}
void UDPMessengerClient::run() {
	char buffer[SIZE];
	while (running) {
		bzero(buffer, sizeof(buffer));
		int rcv = udpSocket->recv(buffer, SIZE);
		if (rcv > 0) {
			cout << buffer << endl;
		} else {
			cout << "Close UDP" << endl;
		}
	}
}

void UDPMessengerClient::stopRunning(){
	this->running = false;
}

UDPMessengerClient::~UDPMessengerClient() {
	// TODO Auto-generated destructor stub
}

} /* namespace npl */
