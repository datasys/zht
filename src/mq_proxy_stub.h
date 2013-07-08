/*
 * mq_proxy_stub.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MQ_PROXY_STUB_H_
#define MQ_PROXY_STUB_H_

#include "proxy_stub.h"

#include "ipc_plus.h"
using namespace IPC;

/*
 *
 */
class MQProxy: public ProtoProxy {
public:
	MQProxy();
	explicit MQProxy(const unsigned int key0);
	virtual ~MQProxy();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

private:
	MsgClient _mc;
};

class MQStub: public ProtoStub {
public:
	MQStub();
	explicit MQStub(const unsigned int key0);
	virtual ~MQStub();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

private:
	MsgServer _ms;
};

#endif /* MQ_PROXY_STUB_H_ */
