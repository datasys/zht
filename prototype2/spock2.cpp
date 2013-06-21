/*
 * spock.c
 *
 *  Created on: Jun 17, 2013
 *      Author: Xiaobing Zhou
 */

#include <errno.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ProxyStubFactory.h"
#include "mpi_proxy_stub.h"

int main(int argc, char **argv) {

	MPIProxy mpiproxy(argc, argv);

	size_t msz;
	char req[IPC_MAX_MSG_SZ];
	char ans[IPC_MAX_MSG_SZ];

	Protocol *stub = ProxyStubFactory::createStub();

	for (;;) {/* never quits! */

		if (!stub->recv(req, msz)) {

			perror("MsgServer::recv");
			exit(1);
		}

		if (!mpiproxy.sendrecv(req, msz, ans, msz)) {

			perror("MPIProxy::sendrecv");
			exit(1);
		}

		if (!stub->send(ans, msz)) {

			perror("MsgServer::send");
			exit(1);
		}

		memset(req, 0, sizeof(req));
		memset(ans, 0, sizeof(ans));
	}

	delete stub;

	return 0;
}
