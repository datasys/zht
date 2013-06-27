/*
 * ProxyStubFactory.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef PROXYSTUBFACTORY_H_
#define PROXYSTUBFACTORY_H_

#include "proxy_stub.h"
/*
 *
 */
class ProxyStubFactory {
public:
	ProxyStubFactory();
	virtual ~ProxyStubFactory();

	static ProtoProxy* createProxy();

	static ProtoStub* createStub();
};

#endif /* PROXYSTUBFACTORY_H_ */
