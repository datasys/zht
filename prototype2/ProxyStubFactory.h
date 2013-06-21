/*
 * ProxyStubFactory.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef PROXYSTUBFACTORY_H_
#define PROXYSTUBFACTORY_H_

#include "Protocol.h"
/*
 *
 */
class ProxyStubFactory {
public:
	ProxyStubFactory();
	virtual ~ProxyStubFactory();

	static Protocol* createProxy();

	static Protocol* createStub();
};

#endif /* PROXYSTUBFACTORY_H_ */
