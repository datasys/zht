/*
 * ProxyFactory.h
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef PROXYFACTORY_H_
#define PROXYFACTORY_H_

#include "ProtocolProxy.h"

/*
 *
 */
class ProxyFactory {
public:
	ProxyFactory();
	virtual ~ProxyFactory();

	static ProtocolProxy* createProxy();
};

#endif /* PROXYFACTORY_H_ */
