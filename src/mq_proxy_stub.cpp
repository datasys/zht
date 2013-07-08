/*
 * mq_proxy_stub.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#include "mq_proxy_stub.h"

MQProxy::MQProxy() :
		_mc(1) {

}

MQProxy::MQProxy(const unsigned int key0) :
		_mc(key0) {
}

MQProxy::~MQProxy() {
}

bool MQProxy::send(const void *sendbuf, const size_t sendcount) {

	return _mc.xmit(sendbuf, sendcount);
}

bool MQProxy::recv(void *recvbuf, size_t &recvcount) {

	return _mc.recv(recvbuf, recvcount);
}

bool MQProxy::sendrecv(const void *sendbuf, const size_t sendcount,
		void *recvbuf, size_t &recvcount) {

	bool sent_bool = _mc.xmit(sendbuf, sendcount);

	bool recv_bool = _mc.recv(recvbuf, recvcount);

	return sent_bool && recv_bool;
}

MQStub::MQStub() :
		_ms(1) {

}

MQStub::MQStub(const unsigned int key0) :
		_ms(key0) {
}

MQStub::~MQStub() {
}

bool MQStub::send(const void *sendbuf, const size_t sendcount) {

	return _ms.xmit(sendbuf, sendcount);
}

bool MQStub::recv(void *recvbuf, size_t &recvcount) {

	return _ms.recv(recvbuf, recvcount);
}
