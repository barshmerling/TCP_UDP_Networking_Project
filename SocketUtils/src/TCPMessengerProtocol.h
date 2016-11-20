/*
 * TCPMessengerProtocol.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

#include "TCPSocket.h"

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3346

#define CLOSE_SESSION_WITH_PEER 		1
#define OPEN_SESSION_WITH_PEER 			2
#define EXIT							3
#define SEND_MSG_TO_PEER				4
#define SESSION_REFUSED					5
#define SESSION_ESTABLISHED				6
#define REGISTER						7
#define LOG_IN							8
#define CURRENT_STATUS					9
#define LIST_OF_USERS					10
#define LIST_CONNECTED_USERS			11
#define LIST_OF_ROOMS					12
#define LIST_USERS_IN_ROOM				13
#define OPEN_OR_CONNECT_ROOM 			14
#define ENTER_TO_ROOM_SUCCESSES			15
#define CLOSE_ROOM						16

#define LOG_IN_SUCCESSES				17
#define LOG_IN_FAILED					18
#define REGISTER_SUCCESSES				19
#define REGISTER_FAILED					20
#define CLIENT_ENTERED_ROOM				21
#define CLIENT_EXITED_FROM_ROOM			22
#define EXIT_ROOM						23
#define MANAGER_LEAVE_ROOM 				24
#define ROOM_CLOSED_BY_MANAGER			25
#define EXIT_ROOM_BACK					26
#define LIST_OF_SESSIONS				27


#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"

namespace npl {
class TCPMessengerProtocol{
public:
	void static sendToServer(int command, const string& data, TCPSocket* sock) {
		char com[4];
		*((int*) com) = htonl(command);
		sock->write(com, 4);
		if (data.length() > 0) {
			char len[4];
			*((int*) len) = htonl(data.length());
			sock->write(len, 4);
			cout << "sending data:" << data << endl << endl << flush;
			sock->write(data.c_str(), data.length()); //data.data()
		}
	}

	void static readFromServer(int& command, string& data, TCPSocket* sock) {
		int com;
		sock->read((char*) &com, 4);
		command = ntohl(com);

		cout << "got command from server:" << command;
		if (command > 0) {
			int len;
			sock->read((char*) &len, 4);
			len = ntohl(len);
			cout<<" cmd len "<<len;
			char buff[len];
			sock->read(buff, len);
			buff[len] = '\0';
			data = buff;
			cout<<" cmd data: "<<data<<endl;

		}
	}

};
}

#endif /* TCPMESSENGERPROTOCOL_H_ */
