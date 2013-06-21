/*
 * serverStub.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

/*
 * spock.c
 *
 *  Created on: Jun 17, 2013
 *      Author: Xiaobing Zhou
 */

#include "StubFactory.h"
#include "MsgShared.h"

#include <stdlib.h>
#include <stdio.h>

int main(void) {

	struct MsgBuf buf;
	buf.size = 200;
	buf.mtext = (char*) calloc(buf.size, sizeof(char));

	printf("spock: ready to receive messages, captain.\n");

	ProtocolStub *ps = StubFactory::createStub();
	/* Spock never quits! */
	for (;;) {

		if (ps->recvMsg(&buf, buf.size) == -1) {

			perror("msgrcv");
			exit(1);
		}

		printf("spock: \"%s\"\n", buf.mtext);

	}

	free(buf.mtext);
	delete ps;

	return 0;
}
