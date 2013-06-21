/*
 * clientProxy.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#include <stdio.h>
#include <string.h>

#include "ProxyFactory.h"

int main(void) {

	printf("Enter lines of text, ^D to quit:\n");

	char buf[200];

	ProtocolProxy *pp = ProxyFactory::createProxy();

	while (fgets(buf, sizeof buf, stdin) != NULL) {

		int len = strlen(buf);

		/* ditch newline at end, if it exists */
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';

		if (pp->sendMsg(&buf, len + 1) == -1) /* +1 for '\0' */
			perror("msgsnd");
	}

	delete pp;

	return 0;
}

