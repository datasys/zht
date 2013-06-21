/*
 * spock.c
 *
 *  Created on: Jun 17, 2013
 *      Author: Xiaobing Zhou
 */

#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "MsgShared.h"

MsgShared ms;

void sigint_handler(int sig) {

//	printf("My parent process ID : %d\n", getpid());

	printf("sigint_handler called\n");

	/*clean up*/
	if (msgctl(ms.get_msqid(), IPC_RMID, NULL) == -1) {

		perror("msgctl");
		exit(1);
	}

}

void init_sig_handler() {

	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0; // or SA_RESTART

	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) == -1) {

		perror("sigaction");
		exit(1);
	}
}

void init_me(void) {

	init_sig_handler();

}

int main(void) {

	init_me();

	struct my_msgbuf buf;

	printf("spock: ready to receive messages, captain.\n");

	/* Spock never quits! */
	for (;;) {

		if (msgrcv(ms.get_msqid(), &buf, sizeof(buf.mtext), 0, 0) == -1) {

			perror("msgrcv");
			exit(1);
		}

		printf("spock: \"%s\"\n", buf.mtext);
	}

	return 0;
}
