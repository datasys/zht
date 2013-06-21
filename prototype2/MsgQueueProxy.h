/*
 * MsgQueueProxy.h
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MSGQUEUEPROXY_H_
#define MSGQUEUEPROXY_H_

#include "ProtocolProxy.h"

#include "MsgShared.h"

/*
 *
 */
class MsgQueueProxy: public ProtocolProxy {
public:
	MsgQueueProxy();
	virtual ~MsgQueueProxy();

	virtual int sendMsg(const void * msgp, size_t msgsz);

	virtual int sendMsg(const MsgDest &msq_dest, const void * msgp,
			size_t msgsz);

private:
	int sendMsgInternal(int fd_dest, const void *msgp, size_t msgsz);

private:
	MsgShared _ms;

};

#endif /* MSGQUEUEPROXY_H_ */
