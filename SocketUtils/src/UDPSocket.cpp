/*
 * UDPSocket.cpp
 *
 *  Created on: Feb 8, 2013
 *      Author: Eliav Menachi
 */

#include "Socket.h"
#include "UDPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

using namespace npl;

/**
 * client UDP socket C'tor
 */
UDPSocket::UDPSocket(){
	  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
}

/**
 * server UDP socket C'tor
 */
UDPSocket::UDPSocket(int port){
	  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	  bind(port);
}

int UDPSocket::recv(char* buffer, int length){
	printf("UDP server receive ...\n");
	bzero((char*)&remoteAddr, sizeof(remoteAddr));
	socklen_t fromSize = sizeof(remoteAddr);
	int total = recvfrom(socket_fd,buffer,length,0,(struct sockaddr *)&remoteAddr, &fromSize);
	if (total < 0)
		cout << "Error receive from" << endl;

	return total;
}

int UDPSocket::sendTo(const string& msg,const string& ip, int port){
	struct sockaddr_in in;
	bzero((char*)& in, sizeof(in));
	in.sin_addr.s_addr = htonl(INADDR_ANY);//	in.sin_addr.s_addr = inet_addr(ip.c_str());
	in.sin_family = (short)AF_INET;
	in.sin_port = htons(port);

	int total = sendto(socket_fd,msg.data(), msg.length(),0,(struct sockaddr *)&in,sizeof(in));

	return total;
}

int UDPSocket::reply(const string& msg){
	cout << "reply " << msg << endl;
	socklen_t fromSize = sizeof(remoteAddr);
	int total = sendto(socket_fd,msg.data(), msg.length(),0,(struct sockaddr*)&remoteAddr,fromSize);
	return total;
}





