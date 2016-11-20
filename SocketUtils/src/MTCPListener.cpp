/*
 * MTCPListener.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: user
 */

#include "MTCPListener.h"

using namespace npl;


/**
 * add sockets to listen to
*/
void MTCPListener::add(TCPSocket* tcpSocket){
	if (std::find(sockets.begin(),sockets.end(),tcpSocket) == sockets.end()) // add to vector if not exist already
				sockets.push_back(tcpSocket);
}

void MTCPListener::add(vector<TCPSocket*> vectorSockets){
	this->sockets = vectorSockets;
}

/**
 * listen to a previously added socket, returns the socket ready for reading
 */
/*
TCPSocket* MTCPListener::listen(int timeout){
	vector<TCPSocket*>::iterator iterBegin = sockets.begin();
	vector<TCPSocket*>::iterator iterEnd = sockets.end();
	fd_set fd;
	int highSize;
	FD_ZERO(&fd);//Clear all entries from the set

	//finding the max socket_fd
	while(iterBegin != iterEnd){
		if (highSize < (*iterBegin)->getSocketFd()) {
			highSize = (*iterBegin)->getSocketFd();
		}
		FD_SET((*iterBegin)->getSocketFd(),&fd);//Add fd to the set
		iterBegin++;
	}

	int result;
	if (timeout > 0) {
		struct timeval timet;
		timet.tv_sec = timeout;
		timet.tv_usec = 0;
		//select the sockets in the set which ready
		result = select(highSize + 1, &fd, NULL, NULL, &timet);
	} else {
		//select the sockets in the set which ready
		result = select(highSize + 1, &fd, NULL, NULL, NULL);
	}

	if (result) {
		//bring the begin iterator to start
		iterBegin = sockets.begin();
		while(iterBegin != iterEnd) {
			TCPSocket* temp = *iterBegin;

			//returns a true if filedes is a member of the file descriptor.
			if(FD_ISSET(temp->getSocketFd(), &fd))
				return temp;
			iterBegin++;
		}
	}
	FD_ZERO(&fd);
	return NULL;
}

*/

TCPSocket* MTCPListener::listen(int timeout){
	fd_set set;
	int nfd = 0;
	FD_ZERO(&set);
	vector<TCPSocket*>::iterator iter = sockets.begin();
	for (; iter != sockets.end(); iter++) {
		TCPSocket* sock = *iter;
		int fd = sock->getSocketFd();
		//cout<<sock->fromAddr()<<endl;
		FD_SET(fd, &set);
		if (fd >= nfd) {
			nfd = fd + 1;
		}
	}

	int rc;
	if (timeout > 0) {
		struct timeval timet;
		timet.tv_sec = timeout;
		timet.tv_usec = 0;
		rc = select(nfd, &set, NULL, NULL, &timet);
	} else {
		rc = select(nfd, &set, NULL, NULL, NULL);
	}
	if (rc < 1) {
		FD_ZERO(&set);
		return NULL;
	}

	iter = sockets.begin();
	for (; iter != sockets.end(); iter++) {
		TCPSocket* sock = *iter;
		int fd = sock->getSocketFd();
		if (FD_ISSET(fd, &set)) {
			FD_ZERO(&set);
			return sock;
		}
	}
	FD_ZERO(&set);
	return NULL;
}
void MTCPListener::remove(TCPSocket* socket) {
	vector<TCPSocket*>::iterator itr = sockets.begin();
	for (; itr != sockets.end(); itr++) {
		if ((*itr) == socket) {
			sockets.erase(itr);
			break;
		}
	}
}
