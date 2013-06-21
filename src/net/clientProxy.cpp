/*
 * clientProxy.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include "ProxyStubFactory.h"

int main(void) {

	Protocol *proxy = ProxyStubFactory::createProxy();

	size_t msz;
	char req[IPC_MAX_MSG_SZ];
	char ans[IPC_MAX_MSG_SZ];

	printf("Enter lines of text, ^D to quit:\n");

	while (fgets(req, sizeof req, stdin) != NULL) {

		int len = strlen(req);

		/* ditch newline at end, if it exists */
		if (req[len - 1] == '\n') {

			printf("len = %d\n", len);
			req[len - 1] = '\0';
		}

		if (!proxy->send(req, len))
			perror("MsgClient::send");

		if (!proxy->recv(ans, msz))
			perror("MsgClient::recv");

		printf("got answer: %s\n", ans);

		memset(req, 0, sizeof(req));
		memset(ans, 0, sizeof(ans));
	}

	delete proxy;
	return 0;
}

