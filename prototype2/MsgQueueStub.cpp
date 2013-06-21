/*
 * MsgQueueStub.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#include "MsgQueueStub.h"

#include <signal.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>

MsgShared MsgQueueStub::_ms = MsgShared();

MsgQueueStub::MsgQueueStub() {

	init_sig_handler();
}

MsgQueueStub::~MsgQueueStub() {
}

int MsgQueueStub::recvMsg(void *msgp, size_t msgsz) {

	return recvMsgInternal(_ms.get_msqid(), msgp, msgsz);
}

int MsgQueueStub::recvMsg(const MsgSrc &msq_src, void * msgp, size_t msgsz) {

	return recvMsgInternal(msq_src.fd_src, msgp, msgsz);
}

int MsgQueueStub::recvMsgInternal(int fd_dest, void *msgp, size_t msgsz) {

	return msgrcv(fd_dest, msgp, msgsz, 0, 0);
}

void MsgQueueStub::sigint_handler(int sig) {

//	printf("My parent process ID : %d\n", getpid());

	printf("sigint_handler called\n");

	/*clean up*/
	if (msgctl(_ms.get_msqid(), IPC_RMID, NULL) == -1) {

		perror("msgctl");
		exit(1);
	}

}

void MsgQueueStub::init_sig_handler() {

	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0; // or SA_RESTART

	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) == -1) {

		perror("sigaction");
		exit(1);
	}
}
