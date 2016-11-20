/*
 * FileInterface.h
 *
 *  Created on: Apr 3, 2016
 *      Author: user
 */

#ifndef FILEINTERFACE_H_
#define FILEINTERFACE_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;
namespace npl {

class FileInterface {
protected:
	int socket_fd;
public:
	virtual void close();
	virtual int write(const char* buffer, int length);
	virtual int read(char* buffer, int length);
	virtual ~FileInterface();
	int getSocketFd();

};

} /* namespace npl */

#endif /* FILEINTERFACE_H_ */
