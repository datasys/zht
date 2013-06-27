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
};

class IPProtoStub: public ProtoStub {
public:
	IPProtoStub();
	virtual ~IPProtoStub();
};

#endif /* IPPROTOCOL_H_ */
