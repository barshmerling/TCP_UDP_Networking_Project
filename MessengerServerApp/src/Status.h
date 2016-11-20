/*
 * Status.h
 *
 *  Created on: Jun 15, 2016
 *      Author: user
 */

#ifndef STATUS_H_
#define STATUS_H_

enum Status {
	DISCONNECTED = 0,
	CONNECTED = 1,
	LOGGED_IN = 2,
	IN_SESSION = 3,
	IN_ROOM = 4
};

#endif /* STATUS_H_ */
