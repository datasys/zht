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
#include <time.h>
#include <string.h>

#include "mpi.h"

#include "ipc_plus.h"
using namespace IPC;

int main(int argc, char **argv) {

	int size, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("spock[%d:%d]: ready to receive messages, captain.\n", size, rank);

	size_t msz;
	char req[IPC_MAX_MSG_SZ];
	char ans[IPC_MAX_MSG_SZ];

	MsgServer ms(1);

	srand(clock());

	for (;;) {/* Spock never quits! */

		if (!ms.recv(req, msz)) {

			perror("MsgServer::recv");
			exit(1);
		}

		int dest = rand() % (size - 1);
		MPI_Send(req, msz, MPI_CHAR, dest, 2, MPI_COMM_WORLD );

		printf("spock, sent to [%d] [%lu] chars: \"%s\"\n", dest, msz, req);

		MPI_Status status;
		MPI_Recv(ans, sizeof(ans), MPI_CHAR, dest, MPI_ANY_TAG, MPI_COMM_WORLD,
				&status);

		if (!ms.xmit(ans, strlen(ans) + 1)) {

			perror("MsgServer::xmit");
			exit(1);
		}

		memset(req, 0, sizeof(req));
		memset(ans, 0, sizeof(ans));
	}

	MPI_Finalize();

	return 0;
}
