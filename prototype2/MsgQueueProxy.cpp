/*
 * MsgQueueProxy.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#include "MsgQueueProxy.h"

#include <sys/msg.h>

MsgQueueProxy::MsgQueueProxy() :
		_ms() {

}

MsgQueueProxy::~MsgQueueProxy() {
}

int MsgQueueProxy::sendMsg(const void * msgp, size_t msgsz) {

	return sendMsgInternal(_ms.get_msqid(), msgp, msgsz);
}

int MsgQueueProxy::sendMsg(const MsgDest &msq_dest, const void * msgp,
		size_t msgsz) {

	return sendMsgInternal(msq_dest.fd_dest, msgp, msgsz);
}

int MsgQueueProxy::sendMsgInternal(int fd_dest, const void *msgp,
		size_t msgsz) {

	struct MsgBuf mbuf;
	mbuf.size = msgsz;
	mbuf.mtext = (char*) msgp;

	return msgsnd(fd_dest, &mbuf, msgsz, 0);
}

