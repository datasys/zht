/*
 * tcp_proxy_stub.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef TCP_PROXY_STUB_H_
#define TCP_PROXY_STUB_H_

#include "IPProtocol.h"

#include "lru_cache.h"
/*
 *
 */
class TCPProxy: public IPProtocol {
public:
	TCPProxy();
	virtual ~TCPProxy();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

	virtual bool teardown();

protected:
	virtual int getSockCached(const string& host, const uint& port);
	virtual int makeClientSocket(const string& host, const uint& port);

private:
	static int CACHE_SIZE;
	static LRUCache<string, int> CONN_CACHE;

};

class TCPStub: public IPProtocol {
public:
	TCPStub();
	virtual ~TCPStub();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

	virtual bool teardown();
};

#endif /* TCP_PROXY_STUB_H_ */
