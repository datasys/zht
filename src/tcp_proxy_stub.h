/*
 * tcp_proxy_stub.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef TCP_PROXY_STUB_H_
#define TCP_PROXY_STUB_H_

#include "ip_proxy_stub.h"

#include "lru_cache.h"
/*
 *
 */
class TCPProxy: public IPProtoProxy {
public:
	TCPProxy();
	virtual ~TCPProxy();

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);
	virtual bool teardown();

protected:
	virtual int getSockCached(const string& host, const uint& port);
	virtual int makeClientSocket(const string& host, const uint& port);
	virtual int recvFrom(int sock, void* recvbuf, int recvbufsize);

private:
	int sendTo(int sock, const void* sendbuf, int sendcount);

private:
	static int CACHE_SIZE;
	static LRUCache<string, int> CONN_CACHE;
};

class TCPStub: public IPProtoStub {
public:
	TCPStub();
	virtual ~TCPStub();

	virtual bool recvsend(ProtoAddr addr, const void *recvbuf);

protected:
	virtual int sendBack(ProtoAddr addr, const void* sendbuf, int sendcount);
};

#endif /* TCP_PROXY_STUB_H_ */
