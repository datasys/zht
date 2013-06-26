/*
 * IPProtocol.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobing Zhou
 */

#include "IPProtocol.h"

#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <iostream>

using namespace std;

IPProtocol::IPProtocol() {
}

IPProtocol::~IPProtocol() {
}

int IPProtocol::reuseSock(int sock) {

	int reuse_addr = 1;
	int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
			sizeof(reuse_addr));
	if (ret < 0) {
		cerr << "resuse socket failed: " << strerror(errno) << endl;
		return -1;
	} else
		return 0;
}
