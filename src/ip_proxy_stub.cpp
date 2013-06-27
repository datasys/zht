/*
 * ip_proxy_stub.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobing Zhou
 */

#include "ip_proxy_stub.h"

#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <iostream>

using namespace std;

IPProtoProxy::IPProtoProxy() {
}

IPProtoProxy::~IPProtoProxy() {
}

int IPProtoProxy::reuseSock(int sock) {

	int reuse_addr = 1;
	int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
			sizeof(reuse_addr));
	if (ret < 0) {
		cerr << "resuse socket failed: " << strerror(errno) << endl;
		return -1;
	} else
		return 0;
}

IPProtoStub::IPProtoStub() {
}

IPProtoStub::~IPProtoStub() {
}
