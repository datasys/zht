/*
 * ProtocolProxy.h
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef PROTOCOLPROXY_H_
#define PROTOCOLPROXY_H_

#include <sys/types.h>
#include "MsgShared.h"
/*
 *
 */
class ProtocolProxy {
public:
	ProtocolProxy();
	virtual ~ProtocolProxy();

	virtual int sendMsg(const void * msgp, size_t msgsz) = 0;

	virtual int sendMsg(const MsgDest &msq_dest, const void * msgp,
			size_t msgsz) = 0;
};

#endif /* PROTOCOLPROXY_H_ */
