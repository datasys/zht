/*
 * StubFactory.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#include "StubFactory.h"

#include "MsgQueueStub.h"

StubFactory::StubFactory() {
}

StubFactory::~StubFactory() {
}

ProtocolStub* StubFactory::createStub() {

	return new MsgQueueStub();
}
