/*
 * mpi_server.c
 *
 *  Created on: Jun 12, 2013
 *      Author: Xiaobing Zhou
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "mpi.h"

#include "ipc_plus.h"
using namespace IPC;

int main(int argc, char **argv) {

	MPI_Status status;

	int size, again, rank;

	size_t msz;
	char req[IPC_MAX_MSG_SZ];
	char ans[IPC_MAX_MSG_SZ];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	while (1) {

		again = 1;

		while (again) {

			fprintf(stderr, "[%d] server loop %d\n", rank, again);

			MPI_Recv(req, sizeof(req), MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
					MPI_COMM_WORLD, &status);

			switch (status.MPI_TAG) {
			case 0:
				fprintf(stderr, "Server recvd terminate cmd\n");

				MPI_Finalize();
				return 0;
			case 2: /* do something */
				fprintf(stderr, "[%d] do something: [%lu] [%s]\n", rank,
						strlen(req), req);

				strcat(req, " [answered]");
				strncpy(ans, req, strlen(req) + 1);

				MPI_Send(ans, strlen(ans) + 1, MPI_CHAR, status.MPI_SOURCE, 0,
						MPI_COMM_WORLD );

				memset(req, 0, sizeof(req));
				memset(ans, 0, sizeof(ans));

				break;
			default:
				/* Unexpected message type */
				MPI_Abort(MPI_COMM_WORLD, 1);
				break;
			}
			++again;

		}
	}

	MPI_Finalize();
}

