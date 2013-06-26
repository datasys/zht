/*
 * mq_proxy_stub.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MQ_PROXY_STUB_H_
#define MQ_PROXY_STUB_H_

#include "Protocol.h"

#include "ipc_plus.h"
using namespace IPC;

/*
 *
 */
class MQProxy: public Protocol {
public:
	MQProxy();
	explicit MQProxy(const unsigned int key0);
	virtual ~MQProxy();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

	virtual bool teardown();

private:
	MsgClient _mc;
};

class MQStub: public Protocol {
public:
	MQStub();
	explicit MQStub(const unsigned int key0);
	virtual ~MQStub();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

	virtual bool teardown();

private:
	MsgServer _ms;
};

#endif /* MQ_PROXY_STUB_H_ */
