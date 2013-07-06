/*
 * ip_server.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: Xiaobing Zhou
 */

#include "ip_server.h"

#include  "ProxyStubFactory.h"

#include <string.h>

IPServer::IPServer() {
}

IPServer::~IPServer() {
}

void IPServer::process(const int& fd, const char * const buf, sockaddr sender) {

	ProtoStub *stub = ProxyStubFactory::createStub();

	ProtoAddr pa;
	pa.fd = fd;
	pa.sender = &sender;

	stub->recvsend(pa, buf);
}

