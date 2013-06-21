/*
 * ProxyStubFactory.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#include "ProxyStubFactory.h"

#include  "mq_proxy_stub.h"

ProxyStubFactory::ProxyStubFactory() {

}

ProxyStubFactory::~ProxyStubFactory() {
}

Protocol* ProxyStubFactory::createProxy() {

	return new MQProxy();
}

Protocol* ProxyStubFactory::createStub() {

	return new MQStub();
}

