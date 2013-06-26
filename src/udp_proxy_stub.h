/*
 * udp_proxy_stub.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef UDP_PROXY_STUB_H_
#define UDP_PROXY_STUB_H_

#include "IPProtocol.h"

/*
 *
 */
class UDPProxy: public IPProtocol {
public:
	UDPProxy();
	virtual ~UDPProxy();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

	virtual bool teardown();

private:
	static int getSockCached(const string& host, const uint& port);
	static int makeClientSocket(const string& host, const uint& port);

private:
	int udpSendTo(int sock, const string &host, uint port, const char* sendbuf,
			int sendcount);
	int udpRecvFrom(int sock, void* recvbuf, int recvbufsize);

private:
	static int UDP_SOCKET;
};

class UDPStub: public IPProtocol {
public:
	UDPStub();
	virtual ~UDPStub();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

	virtual bool teardown();
};

#endif /* UDP_PROXY_STUB_H_ */
