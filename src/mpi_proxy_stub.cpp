/*
 * mpi_proxy_stub.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#include "mpi_proxy_stub.h"

#include "mpi.h"

#include <stdlib.h>
#include <time.h>

MPIProxy::MPIProxy() :
		size(0), rank(-1) {
}

MPIProxy::MPIProxy(int argc, char **argv) {

	srand(clock());

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("spock[%d:%d]: ready to receive messages, captain.\n", size, rank);
}

MPIProxy::~MPIProxy() {

	MPI_Finalize();
}

bool MPIProxy::sendrecv(const void *sendbuf, const size_t sendcount,
		void *recvbuf, size_t &recvcount) {

	int mpi_dest = get_mpi_dest();

	int rs = MPI_Send(sendbuf, sendcount, MPI_CHAR, mpi_dest, 2,
			MPI_COMM_WORLD );

	printf("spock, sent to [%d] %lu char(s): \"%s\"\n", mpi_dest, sendcount,
			(char*) sendbuf);

	size_t msz;
	char ans[IPC_MAX_MSG_SZ];

	MPI_Status status;
	int rr = MPI_Recv(ans, sizeof(ans), MPI_CHAR, mpi_dest, MPI_ANY_TAG,
			MPI_COMM_WORLD, &status);

	memcpy(recvbuf, ans, recvcount = strlen(ans) + 1);

	return rs == MPI_SUCCESS && rr == MPI_SUCCESS;

}

int MPIProxy::get_mpi_dest() {

	return rand() % (size - 1);
}

MPIStub::MPIStub() {

}

MPIStub::~MPIStub() {
}

bool MPIStub::recvsend(void *recvbuf, size_t &recvcount, const void *sendbuf,
		const size_t sendcount) {

	return true;
}
