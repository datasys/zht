/*
 * proxy_stub.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: Xiaobing Zhou
 */

#include "proxy_stub.h"

ProtoAddr::ProtoAddr() :
		fd(-1), sender(0) {
}

ProtoAddr::~ProtoAddr() {
}

ProtoProxy::ProtoProxy() {
}

ProtoProxy::~ProtoProxy() {
}

bool ProtoProxy::send(const void *sendbuf, const size_t sendcount) {

	return false;
}

bool ProtoProxy::recv(void *recvbuf, size_t &recvcount) {

	return false;
}

bool ProtoProxy::sendrecv(const void *sendbuf, const size_t sendcount,
		void *recvbuf, size_t &recvcount) {

	return false;
}

bool ProtoProxy::teardown() {

	return false;
}

ProtoStub::ProtoStub() {
}

ProtoStub::~ProtoStub() {
}

bool ProtoStub::send(const void *sendbuf, const size_t sendcount) {

	return false;
}

bool ProtoStub::recv(void *recvbuf, size_t &recvcount) {

	return false;
}

bool ProtoStub::recvsend(ProtoAddr addr, const void * const recvbuf) {

	return false;
}

bool ProtoStub::teardown() {

	return false;
}
