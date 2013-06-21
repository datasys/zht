/*
 * ProxyFactory.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#include "ProxyFactory.h"

#include "MsgQueueProxy.h"

ProxyFactory::ProxyFactory() {
}

ProxyFactory::~ProxyFactory() {
}

ProtocolProxy* ProxyFactory::createProxy() {

	return new MsgQueueProxy();
}
