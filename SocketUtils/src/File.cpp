/*
 * File.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: user
 */

#include "File.h"

#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fstream>

using namespace npl;

File::File(string path,const char* mode){
	file = fopen(path.data(), mode);
}
File::~File() {
}

