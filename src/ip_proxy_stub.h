/*
 * ip_proxy_stub.h
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef IPPROTOCOL_H_
#define IPPROTOCOL_H_

#include "proxy_stub.h"

#include <string>
using namespace std;

/*
 *
 */
class IPProtoProxy: public ProtoProxy {
public:
	IPProtoProxy();
	virtual ~IPProtoProxy();

protected:
	virtual int reuseSock(int sock);
	virtual int recvFrom(int sock, void* recvbuf, int recvbufsize)= 0;
};

class IPProtoStub: public ProtoStub {
public:
	IPProtoStub();
	virtual ~IPProtoStub();

protected:
	virtual int sendBack(ProtoAddr addr, const void* sendbuf,
			int sendcount) = 0;

};

#endif /* IPPROTOCOL_H_ */
