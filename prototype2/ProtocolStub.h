/*
 * ProtocolStub.h
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef PROTOCOLSTUB_H_
#define PROTOCOLSTUB_H_

#include <sys/types.h>
#include "MsgShared.h"

/*
 *
 */
class ProtocolStub {
public:
	ProtocolStub();
	virtual ~ProtocolStub();

	virtual int recvMsg(void *msgp, size_t msgsz) = 0;

	virtual int recvMsg(const MsgSrc &msq_src, void *msgp, size_t msgsz) = 0;
};

#endif /* PROTOCOLSTUB_H_ */
