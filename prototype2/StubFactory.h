/*
 * StubFactory.h
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef STUBFACTORY_H_
#define STUBFACTORY_H_

#include "ProtocolStub.h"

/*
 *
 */
class StubFactory {
public:
	StubFactory();
	virtual ~StubFactory();

	static ProtocolStub* createStub();
};

#endif /* STUBFACTORY_H_ */
