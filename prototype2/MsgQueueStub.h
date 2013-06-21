/*
 * MsgQueueStub.h
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MSGQUEUESTUB_H_
#define MSGQUEUESTUB_H_

#include "MsgShared.h"

#include "ProtocolStub.h"

/*
 *
 */
class MsgQueueStub: public ProtocolStub {
public:
	MsgQueueStub();
	virtual ~MsgQueueStub();

	virtual int recvMsg(void *msgp, size_t msgsz);

	virtual int recvMsg(const MsgSrc &msq_src, void *msgp, size_t msgsz);

private:
	int recvMsgInternal(int fd_dest, void * msgp, size_t msgsz);

	static void init_sig_handler();

	static void sigint_handler(int sig);

private:
	static MsgShared _ms;
};

#endif /* MSGQUEUESTUB_H_ */
