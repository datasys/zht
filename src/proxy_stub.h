/*
 * proxy_stub.h
 *
 *  Created on: Jun 26, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef PROXY_STUB_H_
#define PROXY_STUB_H_

#include <sys/types.h>

#include "protocol_shared.h"

class ProtoAddr {

public:
	ProtoAddr();
	virtual ~ProtoAddr();

	int fd;
	void *sender;
};

class ProtoProxy {

public:
	ProtoProxy();
	virtual ~ProtoProxy();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

	virtual bool teardown();
};

class ProtoStub {

public:
	ProtoStub();
	virtual ~ProtoStub();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool recvsend(ProtoAddr addr, const void *recvbuf);

	virtual bool teardown();
};
#endif /* PROXY_STUB_H_ */
