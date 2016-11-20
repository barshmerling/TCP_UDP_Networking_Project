/*
 * File.h
 *
 *  Created on: Apr 5, 2016
 *      Author: user
 */

#ifndef FILE_H_
#define FILE_H_

#include "FileInterface.h"
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <stdio.h>

using namespace std;
namespace npl {

class File:public FileInterface {
public:
	::FILE * file;
	File(string path,const char* mode);
	virtual ~File();
};

} /* namespace npl */

#endif /* FILE_H_ */
