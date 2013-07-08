/*
 * mpi_broker.cpp
 *
 *  Created on: Jul 7, 2013
 *      Author: Xiaobing Zhou
 */

#include <errno.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mq_proxy_stub.h"
#include "mpi_proxy_stub.h"

int main(int argc, char **argv) {

	MPIProxy mpiproxy;
	mpiproxy.init(argc, argv);

	size_t msz;
	char req[IPC_MAX_MSG_SZ];
	char ans[IPC_MAX_MSG_SZ];

	MQStub mqstub;

	for (;;) {/* never quits! */

		if (!mqstub.recv(req, msz)) {

			perror("MQStub::recv()");
			exit(1);
		}

		if (!mpiproxy.sendrecv(req, msz, ans, msz)) {

			perror("MPIProxy::sendrecv()");
			exit(1);
		}

		if (!mqstub.send(ans, msz)) {

			perror("MQStub::send()");
			exit(1);
		}

		memset(req, 0, sizeof(req));
		memset(ans, 0, sizeof(ans));
	}

	return 0;
}
