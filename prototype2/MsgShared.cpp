/*
 * MsgShared.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#include "MsgShared.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

MsgShared::MsgShared() :
		_msqid(0) {

	init_msg_queue();
}

MsgShared::~MsgShared() {
}

int MsgShared::get_msqid() {

	return _msqid;
}

void MsgShared::init_msg_queue() {

	key_t key;

	/*todo: give a key not dependent on some filename pleaes*/
	if ((key = ftok("MsgShared.cpp", 'B')) == -1) {

		perror("ftok");
		exit(1);
	}

	/* same key as MsgShared.cpp*/
	if ((_msqid = msgget(key, 0644 | IPC_CREAT)) == -1) { /* connect to the queue */

		perror("msgget");
		exit(1);
	}
}
