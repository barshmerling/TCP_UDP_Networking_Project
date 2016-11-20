/*
 * FileInterface.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: user
 */

#include "FileInterface.h"
#include "TCPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

using namespace std;
using namespace npl;

 void FileInterface::close(){
	cout << "closing socket" << endl;
	shutdown(socket_fd, SHUT_RDWR);
	::close(socket_fd);
}

int FileInterface::write(const char* buffer, int length){
	return ::write(socket_fd, buffer,length);
}

int FileInterface::read(char* buffer, int length){
	return ::read(socket_fd,buffer,length);
}

 FileInterface::~FileInterface(){

}

 int FileInterface::getSocketFd(){
 	return socket_fd;
 }
